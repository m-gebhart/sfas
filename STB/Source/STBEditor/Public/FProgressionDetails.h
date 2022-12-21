// Search for a Star 2023

#pragma once

#include "IDetailCustomization.h"
#include "CoreMinimal.h"
#include "DetailCategoryBuilder.h"
#include "ProgressionData.h"

enum EProgressionDetailsMode
{
	Main,
	Level
};

class FProgressionDetails : public IDetailCustomization 
{
	
public:

	FProgressionDetails();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;

protected:

	FReply OnEditLevelButtonClicked(uint32 Index);
	FReply OnAddLevelButtonClicked(uint32 Index);
	FReply OnRemoveLevelButtonClicked(uint32 Index);
	FReply OnAddCharacterButtonClicked(uint32 Index);
	FReply OnRemoveCharacterButtonClicked(uint32 Index);
	FReply OnBackClicked();

private:
	
	void DoRefresh() const;
	
	void HideUnnecessaryCategories(IDetailLayoutBuilder& DetailBuilder);
	void ShowMain(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category);
	void ShowSelectedLevel(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category);
	void ShowLevelButtons(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category);
	void ShowBackButton(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category);
	void ShowSpacer(IDetailCategoryBuilder& Category);

	TSharedPtr<IPropertyHandle> GetSelectedLevelHandle();
	
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	IDetailLayoutBuilder* CachedDetailBuilder;
	TSharedPtr<IPropertyHandleArray> CachedLevelsHandle;
	TArray<FProgressionLevelData> Levels;
	
	static EProgressionDetailsMode CurrentMode;
	static int SelectedLevel;
};

