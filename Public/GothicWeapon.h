// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GameFramework/Actor.h"
#include "GothicWeapon.generated.h"

UCLASS()
class RPGGAMEPROJECT_API AGothicWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGothicWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetAttackRange() const;
	float GetAttackDamage() const;
	float GetAttackModifier() const;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackDamageMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackModifierMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackModifierMin;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float AttackDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float AttackModifier;

};
