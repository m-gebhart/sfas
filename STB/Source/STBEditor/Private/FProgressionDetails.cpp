// Search for a Star 2023

#include "FProgressionDetails.h"

#include "DetailLayoutBuilder.h"
#include "ProgressionData.h"
#include "DetailWidgetRow.h"

#define LOCTEXT_NAMESPACE "ProgressionDetails"
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

EProgressionDetailsMode FProgressionDetails::CurrentMode = Main;
int FProgressionDetails::SelectedLevel = 0;

FProgressionDetails::FProgressionDetails() : CachedDetailBuilder(nullptr)
{
}

TSharedRef<IDetailCustomization> FProgressionDetails::MakeInstance()
{
	return MakeShareable( new FProgressionDetails );
}

void FProgressionDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	CachedLevelsHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AProgressionData, Levels))->AsArray();
	CachedDetailBuilder = &DetailBuilder;

	if(CustomizedObjects.Num() == 1)
	{
		HideUnnecessaryCategories(DetailBuilder);

		IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Progression Data Details");
	
		if(CurrentMode == Main)
		{
			ShowMain(DetailBuilder, Category);
		}
		else if(CurrentMode == Level)
		{
			ShowSelectedLevel(DetailBuilder, Category);
		}
	}
}

FReply FProgressionDetails::OnEditLevelButtonClicked(uint32 Index)
{
	if(CurrentMode == Main)
	{
		CurrentMode = Level;
		SelectedLevel = Index;
	}
	
	DoRefresh();
	return FReply::Handled();
}

FReply FProgressionDetails::OnAddLevelButtonClicked(uint32 Index)
{
	uint32 NumItems = 0;
	CachedLevelsHandle->GetNumElements(NumItems);
	
	if(Index < static_cast<int>(NumItems))
	{
		CachedLevelsHandle->DuplicateItem(Index);
	}
	else
	{
		CachedLevelsHandle->AddItem();
	}

	DoRefresh();
	return FReply::Handled();
}

FReply FProgressionDetails::OnRemoveLevelButtonClicked(uint32 Index)
{
	CachedLevelsHandle->DeleteItem(Index);
	DoRefresh();
	return FReply::Handled();
}

FReply FProgressionDetails::OnAddCharacterButtonClicked(uint32 Index)
{
	const TSharedPtr<IPropertyHandle> LevelHandle = CachedLevelsHandle->GetElement(SelectedLevel);
	const TSharedPtr<IPropertyHandleArray> CharactersArrayHandle = LevelHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionLevelData, Characters))->AsArray();
	
	uint32 NumItems = 0;
	CharactersArrayHandle->GetNumElements(NumItems);
	
	if(Index < static_cast<int>(NumItems))
	{
		CharactersArrayHandle->DuplicateItem(Index);
	}
	else
	{
		CharactersArrayHandle->AddItem();
	}

	DoRefresh();
	return FReply::Handled();	
}

FReply FProgressionDetails::OnRemoveCharacterButtonClicked(uint32 Index)
{
	const TSharedPtr<IPropertyHandle> LevelHandle = CachedLevelsHandle->GetElement(SelectedLevel);
	const TSharedPtr<IPropertyHandleArray> CharactersArrayHandle = LevelHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionLevelData, Characters))->AsArray();
	
	CharactersArrayHandle->DeleteItem(Index);
	
	DoRefresh();
	return FReply::Handled();
}

FReply FProgressionDetails::OnBackClicked()
{
	if(CurrentMode == Level)
	{
		CurrentMode = Main;
	}
	
	DoRefresh();
	return FReply::Handled();
}

void FProgressionDetails::DoRefresh() const
{
	if(CachedDetailBuilder != nullptr)
	{
		CachedDetailBuilder->ForceRefreshDetails();
	}
}

void FProgressionDetails::HideUnnecessaryCategories(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideCategory("ProgressionData");
	DetailBuilder.HideCategory("Rendering");
	DetailBuilder.HideCategory("Collision");
	DetailBuilder.HideCategory("Input");
	DetailBuilder.HideCategory("Actor");
	DetailBuilder.HideCategory("LOD");
	DetailBuilder.HideCategory("HLOD");
	DetailBuilder.HideCategory("Cooking");
	DetailBuilder.HideCategory("Replication");
}

void FProgressionDetails::ShowMain(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category)
{
	ShowLevelButtons(DetailBuilder, Category);
}

void FProgressionDetails::ShowSelectedLevel(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category)
{
	const FText TitleText = FText::Format(LOCTEXT("TitleText", "Currently Editing Level {0}"), (SelectedLevel + 1));
	
	ShowBackButton(DetailBuilder, Category);
	ShowSpacer(Category);
	
	Category.AddCustomRow(LOCTEXT("LevelTitleTextRow", "Level Title Text"))
		.WholeRowContent()
		[
			SNew(STextBlock)
			.Font(DEFAULT_FONT("BoldFont", 12))
			.Text(TitleText)
		];

	ShowSpacer(Category);
	
	const TSharedPtr<IPropertyHandle> LevelHandle = CachedLevelsHandle->GetElement(SelectedLevel);
	const TSharedPtr<IPropertyHandle> BallBoundsHandle = LevelHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionLevelData, BallBounds));
	const TSharedPtr<IPropertyHandle> RequiredDistanceHandle = LevelHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionLevelData, RequiredDistance));
	const TSharedPtr<IPropertyHandleArray> CharactersArrayHandle = LevelHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionLevelData, Characters))->AsArray();

	uint32 NumElements;
	CharactersArrayHandle->GetNumElements(NumElements);
	
	ShowSpacer(Category);
	Category.AddProperty(BallBoundsHandle);
	Category.AddProperty(RequiredDistanceHandle);
	
	for( uint32 Count = 0; Count < NumElements; ++Count)
	{
		const TSharedPtr<IPropertyHandle> CurrentCharactersHandle = CharactersArrayHandle->GetElement(Count);
		const TSharedPtr<IPropertyHandle> CharacterHandle = CurrentCharactersHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionCharacterData, Character));
		const TSharedPtr<IPropertyHandle> AnimationHandle = CurrentCharactersHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionCharacterData, Animation));
		const TSharedPtr<IPropertyHandle> LocationHandle = CurrentCharactersHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionCharacterData, Location));
		const TSharedPtr<IPropertyHandle> RotationHandle = CurrentCharactersHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionCharacterData, Rotation));
		const TSharedPtr<IPropertyHandle> PositionHandle = CurrentCharactersHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FProgressionCharacterData, Position));

		ShowSpacer(Category);
		Category.AddProperty(CharacterHandle);
		Category.AddProperty(AnimationHandle);
		Category.AddProperty(LocationHandle);
		Category.AddProperty(RotationHandle);
		Category.AddProperty(PositionHandle);

		Category.AddCustomRow(LOCTEXT("CharacterButtonsRow", "Character Buttons"))
			.WholeRowContent()
			[
				SNew(SHorizontalBox)
			
				+ SHorizontalBox::Slot()
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("CharacterAddButtonText", "Duplicate Character"))
					.HAlign(HAlign_Center)
					.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnAddCharacterButtonClicked, Count))
				]

				+ SHorizontalBox::Slot()
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("CharacterRemoveButtonText", "Remove Character"))
					.HAlign(HAlign_Center)
					.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnRemoveCharacterButtonClicked, Count))
				]
			];
	}

	if(NumElements == 0)
	{
		Category.AddCustomRow(LOCTEXT("CharacterAddRow", "Character Add Button"))
			.WholeRowContent()
			[
				SNew(SButton)
				.Text(LOCTEXT("CharacterAddButton", "Create New Character"))
				.HAlign(HAlign_Center)
				.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnAddCharacterButtonClicked, NumElements))
			];
	}
		
}

void FProgressionDetails::ShowLevelButtons(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category)
{
	uint32 NumElements;
	CachedLevelsHandle->GetNumElements(NumElements);
	
	for( uint32 Count = 0; Count < NumElements; ++Count)
	{
		const FString LevelName = FString::Format(TEXT("Level {0}"), { Count });
		const FText ButtonText = FText::Format(LOCTEXT("LevelNameForButton", "Level {0}"), (Count + 1));
	
		Category.AddCustomRow(LOCTEXT("LevelNameButtonRow", "Level Name"))
			.WholeRowContent()
			[
				SNew(SHorizontalBox)
				
				+ SHorizontalBox::Slot()
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.Text(ButtonText)
					.HAlign(HAlign_Center)
					.ToolTipText(LOCTEXT("LevelNameButtonTooltip", "Edit Level"))
					.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnEditLevelButtonClicked, Count))
				]
			
				+ SHorizontalBox::Slot()
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("LevelAddButtonText", "+"))
					.HAlign(HAlign_Center)
					.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnAddLevelButtonClicked, Count))
				]

				+ SHorizontalBox::Slot()
				.Padding(2.0f, 2.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("LevelRemoveButtonText", "-"))
					.HAlign(HAlign_Center)
					.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnRemoveLevelButtonClicked, Count))
				]				
			];
	}

	if(NumElements == 0)
	{
		Category.AddCustomRow(LOCTEXT("CreateAddRow", "Character Add Button"))
			.WholeRowContent()
			[
				SNew(SButton)
				.Text(LOCTEXT("CreateLevelButtonText", "Create First Level"))
				.HAlign(HAlign_Center)
				.OnClicked(FOnClicked::CreateSP(this, &FProgressionDetails::OnAddCharacterButtonClicked, NumElements))
			];
	}	
}

void FProgressionDetails::ShowBackButton(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& Category)
{
	Category.AddCustomRow(LOCTEXT("BackButtonRow", "Back Button"))
	.WholeRowContent()
	[
		SNew(SButton)
		.Text(LOCTEXT("BackButtonText", "Back"))
		.HAlign(HAlign_Center)
		.ToolTipText(LOCTEXT("BackButtonTooltip", "Go Back Up to Level list"))
		.OnClicked(this, &FProgressionDetails::OnBackClicked)
	];
}

void FProgressionDetails::ShowSpacer(IDetailCategoryBuilder& Category)
{
	Category.AddCustomRow(LOCTEXT("SpacerRow", "Spacer"))
	.WholeRowContent()
	[
		SNew(SSpacer)
		.Size(FVector2D(20,20))
	];
}

TSharedPtr<IPropertyHandle> FProgressionDetails::GetSelectedLevelHandle()
{
	return CachedLevelsHandle->GetElement(SelectedLevel);
}

#undef LOCTEXT_NAMESPACE
#undef DEFAULT_FONT