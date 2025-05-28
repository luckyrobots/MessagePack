// ... existing code ...
USTRUCT()
struct FBasedMovementInfo
{
	GENERATED_USTRUCT_BODY()

	/** Unique (within a reasonable timespan) ID of the base component. Can be used to detect changes in the base when the pointer can't replicate, eg during fast shared replication. */
	UPROPERTY()
	uint16 BaseID;

	/** Whether the server says that there is a base. On clients, the component may not have resolved yet. */
	UPROPERTY()
	uint8 bServerHasBaseComponent:1;

	/** Whether rotation is relative to the base or absolute. It can only be relative if location is also relative. */
	UPROPERTY()
	uint8 bRelativeRotation:1;

	/** Whether there is a velocity on the server. Used for forcing replication when velocity goes to zero. */
	UPROPERTY()
	uint8 bServerHasVelocity:1;

// ... existing code ...
	/** Bone name on component, for skeletal meshes. NAME_None if not a skeletal mesh or if bone is invalid. */
	UPROPERTY()
	FName BoneName;

	/** Location relative to the base component. */
	UPROPERTY()
	FVector_NetQuantize100 Location;

	/** Rotation relative to the base component. */
	UPROPERTY()
	FRotator Rotation;

	/** Whether the server says that there is a base. On clients, the component may not have resolved yet. */
	UPROPERTY()
	bool bServerHasBaseComponent_DEPRECATED:1;

	/** Whether rotation is relative to the base or absolute. It can only be relative if location is also relative. */
	UPROPERTY()
	bool bRelativeRotation_DEPRECATED:1;

	/** Whether there is a velocity on the server. Used for forcing replication when velocity goes to zero. */
	UPROPERTY()
	bool bServerHasVelocity_DEPRECATED:1;

	FBasedMovementInfo()
		: BaseID(0)
		, bServerHasBaseComponent(false)
		, bRelativeRotation(false)
		, bServerHasVelocity(false)
		, BoneName(NAME_None)
		, Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, bServerHasBaseComponent_DEPRECATED(false)
		, bRelativeRotation_DEPRECATED(false)
		, bServerHasVelocity_DEPRECATED(false)
	{
	}
};

// ... existing code ...
