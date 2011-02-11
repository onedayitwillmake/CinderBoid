/*
 *  Boid.h
 *  CinderRibbons
 *
 *  Created by Mario Gonzalez on 2/9/11.
 *  Copyright 2011 Whale Island Games. All rights reserved.
 */

#include "cinder/Vector.h"
#define BOID_EPSILON	0.001f
class Boid
{	
public:
	Boid() {};
	Boid( float aMaxSpeed, float aMaxForce );
	
	ci::Vec3f	_velocity;
	ci::Vec3f	_acceleration;
	ci::Vec3f	_steeringForce;
	
	// State
	ci::Vec3f	_position;
	ci::Vec3f	_previousPosition;
	float		_radius;
	
	// Wander
	float		_wanderTheta;	// Modified incrimentally each frmae
	float		_wanderMaxRadius;
	float		_wanderMaxTurningSpeed;
	float		_wanderLookAheadDistance;
	
	
	void		update();
	void		handleBorder();
	
// Accessors
	void		setMaxSpeed(float aSpeed) { _maxSpeed = aSpeed; _maxSpeedSquared = _maxSpeed * _maxSpeed;};
	void		setMaxSteeringForce( float aMaxSteeringForce ) { _maxForce = aMaxSteeringForce; _maxForceSquared = _maxForce*_maxForce; };
	void		setWanderRadiusAndLookAheadDistanceWithMaxTurningAngle( float aWanderRadius, float aLookAheadDistance, float aMaxTurningSpeed ) { _wanderMaxRadius = aWanderRadius; _wanderLookAheadDistance = aLookAheadDistance; _wanderMaxRadius = aMaxTurningSpeed; };
	void		setPosition(float tX, float tY, float tZ) { _position.x = tX; _position.y = tY; _position.z = tZ; };
	ci::Vec3f	getPosition() { return _position; };
	
//	Movement
	void		applyBreakingForce( float aBrakingForce );
	float		wanderWithMultiplier( float aMultiplier );
	// Seeking
	void		seekWithMultiplier( ci::Vec3f aTarget, float aMultiplier );
	void		seekWithingRangeWithMultiplier( ci::Vec3f aTarget, float aMinRange, float aMultiplier );
	void		arriveWithEaseDistanceWithMultiplier( ci::Vec3f aTarget, float anEaseDistance, float aMultiplier );
	// Avoid
	void		fleeIfWithinDistanceWithMultiplier( ci::Vec3f aTarget, float aMinRange, float aMultiplier );
	
	// Flocking
//	void		flock
private:
	float	_maxForce;
	float	_maxForceSquared;
	
	float	_maxSpeed;
	float	_maxSpeedSquared;
	
	bool	_doRotation;
	
	// Steering
	ci::Vec3f	steerTowardsWithEaseDistance( ci::Vec3f aTarget, float anEaseDistance );
};