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

UENUM(BlueprintType)
enum class ECrimsonWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * @class UCrimsonSkillTreeWidget_Display
 * @brief The main container widget for the skill tree UI.
 * @details This widget manages input, holds the graph display, and orchestrates the overall user experience.
 * It is designed as a CommonActivatableWidget for easy integration into game UI flows.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_Display : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Public Methods
	// =============================================================================================================
	/**
	 * @brief Initializes the display with a skill tree instance identified by its type tag.
	 * @details This is the correct way to show a skill tree.
	 * @param SkillTreeTypeTag The tag identifying the skill tree to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display")
	void DisplaySkillTreeByType(FGameplayTag SkillTreeTypeTag);

	/**
	 * @brief Sets the skill tree asset to be displayed and refreshes the graph.
	 * @deprecated This function is unsafe as it can be accidentally used with a template asset. Use DisplaySkillTreeByType instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display", meta = (DeprecatedFunction, DeprecationMessage = "Use DisplaySkillTreeByType instead to ensure the correct runtime instance is used."))
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

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Display")
	void SetSkillTreeTypeTag(const FGameplayTag NewTypeTag);
	
protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UCommonActivatableWidget Overrides
	// =============================================================================================================
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	/**
	 * @brief Called when the widget is activated.
	 */
	virtual void NativeOnActivated() override;

	/**
	 * @brief Called when the widget is deactivated.
	 */
	virtual void NativeOnDeactivated() override;
	
	/**
 	* @brief Called by a delegate when the player controller possesses a new pawn.
 	* @details This is the key to re-initializing the UI after a respawn.
 	* @param OldPawn The pawn that was previously possessed.
 	* @param NewPawn The pawn that is now possessed.
 	*/
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	/**
	 * @brief Specifies the desired input configuration for this widget when it is active.
	 * @return The desired UI input configuration.
	 */
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	// ~Input Handling
	// =============================================================================================================
	/**
	 * @brief Binds the Enhanced Input actions to their handler functions.
	 */
	void BindInputActions();

	/**
	 * @brief Unbinds all input actions.
	 */
	void UnbindInputActions();

	/**
	 * @brief Handles the start of the pan toggle input action.
	 * @param Value The input action value.
	 */
	void HandlePanToggleStarted(const struct FInputActionValue& Value);

	/**
	 * @brief Handles the completion of the pan toggle input action.
	 * @param Value The input action value.
	 */
	void HandlePanToggleCompleted(const struct FInputActionValue& Value);

	/**
	 * @brief Handles the zoom input action.
	 * @param Value The input action value, typically the mouse wheel delta.
	 */
	void HandleZoomAction(const struct FInputActionValue& Value);

	/**
	 * @brief Handles the primary action on a node (e.g., activate, level up).
	 * @param Value The input action value.
	 */
	void HandleNodePrimaryAction(const FInputActionValue& Value);

	/**
	 * @brief Handles the secondary action on a node (e.g., deactivate, level down).
	 * @param Value The input action value.
	 */
	void HandleNodeSecondaryAction(const FInputActionValue& Value);

	/**
	 * @brief The function called by a timer to smoothly update the graph's pan position.
	 */
	UFUNCTION()
	void UpdatePanOnTimer();

	// ~Data Handling
	// =============================================================================================================
	/**
	 * @brief Called when the manager's replicated data has been updated.
	 */
	UFUNCTION()
	void OnManagerDataUpdated();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Display")
	void ResetSkillTree() const;
	
	/**
	 * @brief A robust way to get the player's skill tree manager component.
	 * @return A pointer to the UCrimsonSkillTreeManager, or nullptr if not found.
	 */
	UCrimsonSkillTreeManager* GetSkillTreeManager() const;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~UMG Widget Bindings
	// =============================================================================================================
	/** @brief Reference to the graph widget within this display. Must be bound in the UMG editor. */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree|Visuals", meta = (BindWidget))
	TObjectPtr<UCrimsonSkillTreeWidget_Graph> SkillTreeGraph;

	/** @brief The skill tree instance currently being displayed. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill Tree")
	TObjectPtr<UCrimsonSkillTree> CurrentSkillTree;

	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input")
	ECrimsonWidgetInputMode InputConfig = ECrimsonWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
	/** @brief Sets the input priority of the IMC*/
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input")
	int32 InputPriority = 0;
	
	/** @brief The update frequency for panning, in seconds. (e.g., 1.0 / 60.0 for 60 FPS). */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input|Panning", meta = (ClampMin = "0.01"))
	float PanUpdateInterval;

public:
	// ~Runtime State
	// =============================================================================================================

	/** @brief Stores the tag of the skill tree we were asked to display, so we can retry if data isn't ready. */
	UPROPERTY(Transient)
	FGameplayTag RequestedSkillTreeTypeTag;
	
	// ~Configuration Properties
	// =============================================================================================================
	/** @brief The Input Mapping Context for all skill tree interactions. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input")
	TObjectPtr<UInputMappingContext> SkillTreeInputMappingContext;
	
	/** @brief Toggles panning mode (e.g., bound to Right Mouse Button). */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input|Actions")
	TObjectPtr<UInputAction> PanToggleAction;

	/** @brief Handles zooming in and out (e.g., bound to Mouse Wheel). */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input|Actions")
	TObjectPtr<UInputAction> ZoomAction;

	/** @brief Handles the primary action on a node (Activate/Increment Level, e.g., Left Mouse Button). */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input|Actions")
	TObjectPtr<UInputAction> NodePrimaryAction;

	/** @brief Handles the secondary action on a node (Deactivate/Decrement Level, e.g., Right Mouse Button). */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Input|Actions")
	TObjectPtr<UInputAction> NodeSecondaryAction;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	
	// ~Runtime State
	// =============================================================================================================

	/** @brief A weak pointer to the node widget currently under the mouse cursor. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Node> HoveredNodeWidget;

	/** @brief Timer handle for the smooth panning update loop. */
	FTimerHandle PanUpdateTimerHandle;

	/** @brief Flag indicating if panning is currently active. */
	bool bIsPanningActive = false;

	/** @brief The mouse position in viewport space when panning started. */
	FVector2D PanStartMouse_ViewportSpace;

	/** @brief The initial translation of the canvas when panning started. */
	FVector2D InitialCanvasTranslationForPan;
	
};
