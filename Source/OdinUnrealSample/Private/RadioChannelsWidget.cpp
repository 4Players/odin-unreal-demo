#include "RadioChannelsWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"

void URadioChannelsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Team1Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleTeam1Clicked);
    Team2Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleTeam2Clicked);
    Listen1Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleListen1Clicked);
    Listen2Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleListen2Clicked);
    Listen3Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleListen3Clicked);
    Talk1Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleTalk1Clicked);
    Talk2Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleTalk2Clicked);
    Talk3Button->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleTalk3Clicked);

    RefreshAppearance();
}

void URadioChannelsWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    RefreshAppearance();
}

void URadioChannelsWidget::SelectTeam(int32 Team)
{
    if ((Team != 1 && Team != 2) || SelectedTeam == Team)
    {
        return;
    }

    SelectedTeam = Team;
    bListen1Active = true;
    bListen2Active = false;
    bListen3Active = false;
    bTalk1Active = true;
    bTalk2Active = false;
    bTalk3Active = false;

    RefreshAppearance();
    OnTeamSelected.Broadcast(SelectedTeam);
    OnSelectionChanged.Broadcast(SelectedTeam, GetActiveListenChannels(), GetActiveTalkChannels());
}

void URadioChannelsWidget::SetListenActive(int32 Channel, bool bActive)
{
    bool* State = nullptr;

    switch (Channel)
    {
    case 1: State = &bListen1Active; break;
    case 2: State = &bListen2Active; break;
    case 3: State = &bListen3Active; break;
    default: return;
    }

    if (*State == bActive)
    {
        return;
    }

    *State = bActive;

    RefreshAppearance();
    OnListenChanged.Broadcast(Channel, bActive);
    OnSelectionChanged.Broadcast(SelectedTeam, GetActiveListenChannels(), GetActiveTalkChannels());
}

void URadioChannelsWidget::SetTalkActive(int32 Channel, bool bActive)
{
    bool* State = nullptr;

    switch (Channel)
    {
    case 1: State = &bTalk1Active; break;
    case 2: State = &bTalk2Active; break;
    case 3: State = &bTalk3Active; break;
    default: return;
    }

    if (*State == bActive)
    {
        return;
    }

    *State = bActive;

    RefreshAppearance();
    OnTalkChanged.Broadcast(Channel, bActive);
    OnSelectionChanged.Broadcast(SelectedTeam, GetActiveListenChannels(), GetActiveTalkChannels());
}

void URadioChannelsWidget::SetRadioBackgroundColor(FLinearColor Color)
{
    BackgroundColor = Color;

    RefreshAppearance();
}

TArray<int32> URadioChannelsWidget::GetActiveListenChannels() const
{
    TArray<int32> ActiveChannels;

    if (bListen1Active) ActiveChannels.Add(1);
    if (bListen2Active) ActiveChannels.Add(2);
    if (bListen3Active) ActiveChannels.Add(3);

    return ActiveChannels;
}

TArray<int32> URadioChannelsWidget::GetActiveTalkChannels() const
{
    TArray<int32> ActiveChannels;

    if (bTalk1Active) ActiveChannels.Add(1);
    if (bTalk2Active) ActiveChannels.Add(2);
    if (bTalk3Active) ActiveChannels.Add(3);

    return ActiveChannels;
}

void URadioChannelsWidget::HandleTeam1Clicked() { SelectTeam(1); }
void URadioChannelsWidget::HandleTeam2Clicked() { SelectTeam(2); }
void URadioChannelsWidget::HandleListen1Clicked() { SetListenActive(1, !bListen1Active); }
void URadioChannelsWidget::HandleListen2Clicked() { SetListenActive(2, !bListen2Active); }
void URadioChannelsWidget::HandleListen3Clicked() { SetListenActive(3, !bListen3Active); }
void URadioChannelsWidget::HandleTalk1Clicked() { SetTalkActive(1, !bTalk1Active); }
void URadioChannelsWidget::HandleTalk2Clicked() { SetTalkActive(2, !bTalk2Active); }
void URadioChannelsWidget::HandleTalk3Clicked() { SetTalkActive(3, !bTalk3Active); }

void URadioChannelsWidget::RefreshAppearance()
{
    const FLinearColor Active(0.08f, 0.62f, 0.82f, 1.0f);
    const FLinearColor Inactive(0.13f, 0.18f, 0.23f, 1.0f);

    if (Backdrop) Backdrop->SetBrushColor(BackgroundColor);
    if (Team1Button) Team1Button->SetBackgroundColor(SelectedTeam == 1 ? Active : Inactive);
    if (Team2Button) Team2Button->SetBackgroundColor(SelectedTeam == 2 ? Active : Inactive);
    if (Listen1Button) Listen1Button->SetBackgroundColor(bListen1Active ? Active : Inactive);
    if (Listen2Button) Listen2Button->SetBackgroundColor(bListen2Active ? Active : Inactive);
    if (Listen3Button) Listen3Button->SetBackgroundColor(bListen3Active ? Active : Inactive);
    if (Talk1Button) Talk1Button->SetBackgroundColor(bTalk1Active ? Active : Inactive);
    if (Talk2Button) Talk2Button->SetBackgroundColor(bTalk2Active ? Active : Inactive);
    if (Talk3Button) Talk3Button->SetBackgroundColor(bTalk3Active ? Active : Inactive);
}
