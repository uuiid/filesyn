// Fill out your copyright notice in the Description page of Project Settings.

#include "fireLight.h"

#include "Components/PointLightComponent.h"
// Sets default values
AfireLight::AfireLight() {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  //添加一些默认组件
  p_LocalLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LocalLight"));
  p_LocalLight->SetupAttachment(RootComponent);

  //添加一些曲线点
  p_LocalLightCurve = FRuntimeFloatCurve{};
  auto k_curve      = p_LocalLightCurve.GetRichCurve();
  k_curve->AutoSetTangents();
  k_curve->SetKeyTangentMode(k_curve->AddKey(0.0, 0.8), ERichCurveTangentMode::RCTM_Auto);
  k_curve->SetKeyTangentMode(k_curve->AddKey(0.5, 1.0), ERichCurveTangentMode::RCTM_Auto);
  k_curve->SetKeyTangentMode(k_curve->AddKey(1.0, 0.8), ERichCurveTangentMode::RCTM_Auto);
}

// Called when the game starts or when spawned
void AfireLight::BeginPlay() {
  Super::BeginPlay();
}

// Called every frame
void AfireLight::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // 获得物体创建时间
  auto time = GetGameTimeSinceCreation();

  // 生成噪波函数
  auto noise = FMath::PerlinNoise1D(time * Speed);
  noise      = FMath::Abs(noise);
  // FMath::DivideAndRoundNearest()

  // 获得最大和最小猪
  auto tmp_min = FMath::Min(luminanceMin, luminanceMax);
  auto tmp_max = FMath::Max(luminanceMin, luminanceMax);
  noise        = FMath::GetMappedRangeValueClamped({0, 1}, {tmp_min, tmp_max}, noise) * tmp_max;

  // 获得曲线变化
  float tmp{1.0};
  auto time_model = FMath::Modf(time, &tmp);
  noise           = p_LocalLightCurve.GetRichCurveConst()->Eval(time_model) * noise;

  // 设置灯光亮度
  p_LocalLight->SetIntensity(noise);
  // p_LocalLight->SetLightBrightness(noise * 1000);
  // debug宏
  // UE_LOG(LogTemp, Log, TEXT("%f"), time_modesl);
}

bool AfireLight::ShouldTickIfViewportsOnly() const {
  return true;
}