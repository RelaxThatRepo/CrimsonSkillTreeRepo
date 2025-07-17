// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h" // Original include

class IToolkitHost; // Forward declaration for IToolkitHost used in OpenAssetEditor
/**
 * @class FAssetTypeActions_CrimsonSkillTree
 * @brief Defines editor actions associated with the UCrimsonSkillTree asset type.
 * This includes its name in menus, color, supported class, and how to open its custom editor.
 */
class CRIMSONSKILLTREEEDITOR_API FAssetTypeActions_CrimsonSkillTree: public FAssetTypeActions_Base
{
public:
	/**
	 * @brief Constructor.
	 * @param InAssetCategory The category this asset type belongs to in the content browser.
	 */
	FAssetTypeActions_CrimsonSkillTree(EAssetTypeCategories::Type InAssetCategory);

	//~ FAssetTypeActions_Base Interface
	/** @return The display name of this asset type. */
	virtual FText GetName() const override;

	/** @return The color associated with this asset type in the content browser. */
	virtual FColor GetTypeColor() const override;

	/** @return The UClass that this asset type action supports (UCrimsonSkillTree). */
	virtual UClass* GetSupportedClass() const override;

	/**
	 * @brief Called when the asset is opened (e.g., by double-clicking in the content browser).
	 * This function is responsible for creating and initializing the custom asset editor for UCrimsonSkillTree.
	 * @param InObjects Array of UObject assets to open. Should contain UCrimsonSkillTree instances.
	 * @param EditWithinLevelEditor Optional toolkit host, used when opening the asset in a level-centric editor mode.
	 */
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	
	/** @return The asset categories this asset type belongs to. */
	virtual uint32 GetCategories() override;
	
	//~ End FAssetTypeActions_Base Interface

private:
	/** @brief The asset category identifier provided during construction. */
	EAssetTypeCategories::Type MyAssetCategory;
};
