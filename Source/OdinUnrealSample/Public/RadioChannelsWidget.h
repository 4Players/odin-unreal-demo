#pragma once

#include "Blueprint/UserWidget.h"
#include "RadioChannelsWidget.generated.h"

class UBorder;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRadioTeamSelected, int32, Team);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRadioListenChanged, int32, Channel, bool, bActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRadioTalkChanged, int32, Channel, bool, bActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRadioSelectionChanged, int32, Team, const TArray<int32>&, ListenChannels, const TArray<int32>&, TalkChannels);

UCLASS(Blueprintable)
class ODINUNREALSAMPLE_API URadioChannelsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Radio|Events")
    FRadioTeamSelected OnTeamSelected;

    UPROPERTY(BlueprintAssignable, Category = "Radio|Events")
    FRadioListenChanged OnListenChanged;

    UPROPERTY(BlueprintAssignable, Category = "Radio|Events")
    FRadioTalkChanged OnTalkChanged;

    UPROPERTY(BlueprintAssignable, Category = "Radio|Events")
    FRadioSelectionChanged OnSelectionChanged;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    int32 SelectedTeam = 1;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bListen1Active = true;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bListen2Active = false;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bListen3Active = false;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bTalk1Active = true;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bTalk2Active = false;

    UPROPERTY(BlueprintReadOnly, Category = "Radio|State")
    bool bTalk3Active = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio|Appearance", meta = (ExposeOnSpawn = "true"))
    FLinearColor BackgroundColor = FLinearColor(0.025f, 0.05f, 0.075f, 0.94f);

    UFUNCTION(BlueprintCallable, Category = "Radio")
    void SelectTeam(int32 Team);

    UFUNCTION(BlueprintCallable, Category = "Radio")
    void SetListenActive(int32 Channel, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Radio")
    void SetTalkActive(int32 Channel, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Radio|Appearance")
    void SetRadioBackgroundColor(FLinearColor Color);

    UFUNCTION(BlueprintPure, Category = "Radio")
    TArray<int32> GetActiveListenChannels() const;

    UFUNCTION(BlueprintPure, Category = "Radio")
    TArray<int32> GetActiveTalkChannels() const;

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> Backdrop;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Team1Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Team2Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Listen1Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Listen2Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Listen3Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Talk1Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Talk2Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Talk3Button;

    UFUNCTION()
    void HandleTeam1Clicked();

    UFUNCTION()
    void HandleTeam2Clicked();

    UFUNCTION()
    void HandleListen1Clicked();

    UFUNCTION()
    void HandleListen2Clicked();

    UFUNCTION()
    void HandleListen3Clicked();

    UFUNCTION()
    void HandleTalk1Clicked();

    UFUNCTION()
    void HandleTalk2Clicked();

    UFUNCTION()
    void HandleTalk3Clicked();

    void RefreshAppearance();
};
