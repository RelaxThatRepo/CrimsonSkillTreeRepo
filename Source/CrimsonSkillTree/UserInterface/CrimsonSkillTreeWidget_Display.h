// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTreeWidget_Display.generated.h"

class UCrimsonSkillTree;
class UCrimsonSkillTreeManager;
class UCrimsonSkillTreeWidget_Graph;
class UCrimsonSkillTreeWidget_Node;
class UInputMappingContext;
class UInputAction;

/**
 * @class UCrimsonSkillTreeWidget_Display
 * @brief The main container widget for the skill tree UI.
 * This widget manages input, holds the graph display, and orchestrates the overall user experience.
 * It is designed as a CommonActivatableWidget for easy integration into game UI flows.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_Display : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	//~ Public Methods
	/**
	 * @brief Initializes the display with a skill tree instance identified by its type tag.
	 * This is the correct way to show a skill tree.
	 * @param SkillTreeTypeTag The tag identifying the skill tree to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display")
	void DisplaySkillTreeByType(FGameplayTag SkillTreeTypeTag);

	/**
	 * @brief Sets the skill tree asset to be displayed and refreshes the graph.
	 * @deprecated This function is unsafe as it can be accidentally used with a template asset. Use DisplaySkillTreeByType instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display", meta=(DeprecatedFunction, DeprecationMessage="Use DisplaySkillTreeByType instead to ensure the correct runtime instance is used."))
	void SetSkillTree(UCrimsonSkillTree* InSkillTree);

	/**
	 * @brief Refreshes the graph display using the current skill tree asset.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display")
	void RefreshDisplay();

	/**
	 * @brief Sets the currently hovered node widget. Called by the node widget itself on mouse enter.
	 * @param NodeWidget The node widget that is being hovered.
	 */
	void SetHoveredNodeWidget(UCrimsonSkillTreeWidget_Node* NodeWidget);

	/**
	 * @brief Clears the hovered node widget. Called by the node widget on mouse leave.
	 * @param NodeWidget The node widget that is no longer being hovered.
	 */
	void ClearHoveredNodeWidget(const UCrimsonSkillTreeWidget_Node* NodeWidget);
	//~ End Public Methods
	
protected:
	//~ UCommonActivatableWidget Overrides
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~ End UCommonActivatableWidget Overrides

	//~ Input Handling
	/**
	 * @brief Binds the Enhanced Input actions to their handler functions.
	 */
	void BindInputActions();

	/**
	 * @brief Unbinds all input actions.
	 */
	void UnbindInputActions();

	/** Input Action Handlers */
	void HandlePanToggleStarted(const struct FInputActionValue& Value);
	void HandlePanToggleCompleted(const struct FInputActionValue& Value);
	void HandleZoomAction(const struct FInputActionValue& Value);
	void HandleNodePrimaryAction(const FInputActionValue& Value);
	void HandleNodeSecondaryAction(const FInputActionValue& Value);

	/**
	 * @brief The function called by a timer to smoothly update the graph's pan position.
	 */
	UFUNCTION()
	void UpdatePanOnTimer();
	//~ End Input Handling

	//~ Helper Functions
	/**
	 * @brief A robust way to get the player's skill tree manager component.
	 * @return A pointer to the UCrimsonSkillTreeManager, or nullptr if not found.
	 */
	UCrimsonSkillTreeManager* GetSkillTreeManager() const;
	//~ End Helper Functions

protected:
	//~ UMG Widget Bindings
	// Reference to the graph widget within this display. Must be bound in the UMG editor.
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Display|Visuals", meta = (BindWidget))
	TObjectPtr<UCrimsonSkillTreeWidget_Graph> SkillTreeGraph;
	//~ End UMG Widget Bindings

	//~ Configuration Properties
	// The skill tree instance currently being displayed.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill Tree Display")
	TObjectPtr<UCrimsonSkillTree> CurrentSkillTree;

	// The Input Mapping Context for all skill tree interactions.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> SkillTreeInputMappingContext;

	// Toggles panning mode (e.g., bound to Right Mouse Button).
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> PanToggleAction;

	// Handles zooming in and out (e.g., bound to Mouse Wheel).
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> ZoomAction;

	// Handles the primary action on a node (Activate/Increment Level, e.g., Left Mouse Button).
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> NodePrimaryAction;

	// Handles the secondary action on a node (Deactivate/Decrement Level, e.g., Right Mouse Button).
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> NodeSecondaryAction;

	// The update frequency for panning, in seconds. (e.g., 1.0 / 60.0 for 60 FPS).
	UPROPERTY(EditDefaultsOnly, Category = "Panning", meta = (ClampMin = "0.01"))
	float PanUpdateInterval;
	//~ End Configuration Properties

private:
	//~ Runtime State
	// A weak pointer to the node widget currently under the mouse cursor.
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Node> HoveredNodeWidget;
	
	// Timer handle for the smooth panning update loop.
	FTimerHandle PanUpdateTimerHandle;

	// State variables for calculating the pan delta.
	bool bIsPanningActive = false;
	FVector2D PanStartMouse_ViewportSpace;
	FVector2D InitialCanvasTranslationForPan;
	//~ End Runtime State
};
