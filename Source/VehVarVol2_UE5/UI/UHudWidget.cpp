// 


#include "UHudWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHudWidget::ShowCrosshair()
{
	CrossHairImage->SetVisibility(ESlateVisibility::Visible);
}

void UHudWidget::HideCrosshair()
{
	CrossHairImage->SetVisibility(ESlateVisibility::Hidden);
}

void UHudWidget::UpdateHealthBar(float currentHealth, float maxHealth)
{
	float healthPercent = FMath::Clamp(currentHealth / maxHealth, 0.0f, 1.0f);

	if (HealthProgressBar)
		HealthProgressBar->SetPercent(healthPercent);
}
