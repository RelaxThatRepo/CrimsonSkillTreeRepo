#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Misc/ScopeRWLock.h" // For thread safety if needed, though likely single-threaded context

// Helper to manage lifetime of struct data if complex structs are used.
// For simple structs, direct storage might be fine. FStructOnScope is good for UStructs.
#include "UObject/StructOnScope.h"

#include "SimulationDataCollector.generated.h"
/**
 * Collects various types of simulation data (as USTRUCTs) from NodeEvents.
 * ActivationConditions can then query this collector for data types they understand.
 */
USTRUCT(BlueprintType)
struct FSimulationDataCollector
{
    GENERATED_BODY()
    
private:
    // Stores UStruct type -> array of instances of that struct (as FStructOnScope for proper UStruct handling)
    TMap<const UScriptStruct*, TArray<TSharedPtr<FStructOnScope>>> CollectedData;
    // If only one instance per type is expected, TMap<const UScriptStruct*, TSharedPtr<FStructOnScope>> is simpler.
    // Using TArray allows an event to provide multiple pieces of data of the same type if needed.

public:
    FSimulationDataCollector() = default;

    /**
     * Adds a piece of simulation data (any USTRUCT) to the collector.
     * The data is copied.
     */
    template <typename FStructType>
    void AddData(const FStructType& DataPayload)
    {
        const UScriptStruct* StructType = FStructType::StaticStruct(); // Ensure this works for USTRUCTs
        if (!StructType) return;

        TSharedPtr<FStructOnScope> NewDataInstance = MakeShared<FStructOnScope>(StructType);
        StructType->CopyScriptStruct(NewDataInstance->GetStructMemory(), &DataPayload);

        TArray<TSharedPtr<FStructOnScope>>& DataList = CollectedData.FindOrAdd(StructType);
        DataList.Add(NewDataInstance);
    }

    /**
     * Retrieves the first piece of simulation data of the specified USTRUCT type.
     * @return Pointer to the data if found (valid for lifetime of collector/shared_ptr), nullptr otherwise.
     */
    template <typename FStructType>
    const FStructType* GetData() const
    {
        const UScriptStruct* StructType = FStructType::StaticStruct();
        if (!StructType) return nullptr;

        const TArray<TSharedPtr<FStructOnScope>>* DataListPtr = CollectedData.Find(StructType);
        if (DataListPtr && DataListPtr->Num() > 0 && (*DataListPtr)[0].IsValid())
        {
            return reinterpret_cast<const FStructType*>((*DataListPtr)[0]->GetStructMemory());
        }
        return nullptr;
    }

    /**
     * Retrieves all pieces of simulation data of the specified USTRUCT type.
     * @param OutDataArray Array to populate with pointers to the data. Pointers valid for collector lifetime.
     */
    template <typename FStructType>
    void GetAllDataOfType(TArray<const FStructType*>& OutDataArray) const
    {
        OutDataArray.Empty();
        const UScriptStruct* StructType = FStructType::StaticStruct();
        if (!StructType) return;

        const TArray<TSharedPtr<FStructOnScope>>* DataListPtr = CollectedData.Find(StructType);
        if (DataListPtr)
        {
            for (const TSharedPtr<FStructOnScope>& DataInstance : *DataListPtr)
            {
                if (DataInstance.IsValid())
                {
                    OutDataArray.Add(reinterpret_cast<const FStructType*>(DataInstance->GetStructMemory()));
                }
            }
        }
    }

    void Clear()
    {
        CollectedData.Empty();
    }
};