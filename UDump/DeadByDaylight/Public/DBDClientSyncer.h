#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnInfo.h"
#include "DBDClientSyncer.generated.h"

class UPaperTileMap;

UCLASS(Blueprintable)
class ADBDClientSyncer : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<UClass*> _actorsToSpawnLocally;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FSpawnInfo> _currentQueue;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UPaperTileMap* _tileMap;

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SyncSeed(int32 Seed, UPaperTileMap* TileMap);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SyncFinalLocallySpawnCount(int16 numLocal);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SyncActorToSpawnLocally(const TArray<FSpawnInfo>& spawnInfos, int16 startIndex);

public:
	ADBDClientSyncer();
};

FORCEINLINE uint32 GetTypeHash(const ADBDClientSyncer) { return 0; }
