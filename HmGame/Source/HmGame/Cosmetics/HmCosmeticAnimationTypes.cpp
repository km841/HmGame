#include "HmCosmeticAnimationTypes.h"

USkeletalMesh* FHmAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
    // MeshRule을 순회하며, CosmeticTags의 조건에 맞는 Rule을 찾아 SkeletalMesh를 반환한다.
    for (const FHmAnimBodyStyleSelectionEntry& Rule : MeshRules)
    {
        if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
        {
            return Rule.Mesh;
        }
    }

    return DefaultMesh;
}