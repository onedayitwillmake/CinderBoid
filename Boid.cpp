/*
 *  Boid.h
 *  CinderRibbons
 *
 *  Created by Mario Gonzalez on 2/9/11.
 *  Copyright 2011 Whale Island Games. All rights reserved.
 */

#include "Boid.h" 

Boid::Boid( float aMaxSpeed, float aMaxSteeringForce )
{
	setMaxSpeed( aMaxSpeed );
	setMaxSteeringForce( aMaxSteeringForce );
}

void Boid::update()
{
	_previousPosition.x = _position.x;
	_previousPosition.y = _position.y;

	// Add current velocity
	_velocity += _acceleration;
	
	// Clamp velocity
	float maxVelSquared = _velocity.lengthSquared();
	if(maxVelSquared >= _maxSpeedSquared) {
		_velocity.normalize();
		_velocity *= _maxSpeed;
	}
	
	// Apply velocity
	_position += _velocity;
	
	// Rotate
	if(_doRotation) {
		// TODO: Rotation junk
	}
	
	// Reset acceleration
	_acceleration = ci::Vec3f::zero();
}

ci::Vec3f Boid::steerTowardsWithEaseDistance( ci::Vec3f aTarget, float anEaseDistance = -1.0f  )
{
	_steeringForce = ci::Vec3f( aTarget );
	_steeringForce -= _position;
	
	// Steer towards target if not already there
	float distanceSquared = _steeringForce.lengthSquared();
	if(distanceSquared > BOID_EPSILON)
	{
		// Apply less strongly as we approach
		if(anEaseDistance > 1.0f && distanceSquared < anEaseDistance * anEaseDistance ) 
		{
			float distance = sqrtf( distanceSquared );
			_steeringForce *= _maxSpeed * ( distance / anEaseDistance );
		} 
		else // Apply fully 100% of the time
		{
			_steeringForce *= _maxSpeed;
		}
		
		// Offset current velocity from steering force
		_steeringForce -= _velocity;

		// Cap steering force
		float steeringForceSquared = _steeringForce.lengthSquared();
		if( steeringForceSquared > _maxForceSquared ) {
			_steeringForce.normalize();
			_steeringForce *= _maxForce;
		}
	}
	
	return _steeringForce;
}

void Boid::applyBreakingForce( float aBrakingForce )
{
	_velocity *= 1.0f - aBrakingForce;
}

void Boid::seekWithMultiplier( ci::Vec3f aTarget, float aMultiplier = 1.0f )
{
	// Apply steering force
	_steeringForce = steerTowardsWithEaseDistance( aTarget );
	_steeringForce *= aMultiplier;
	
	// Append to acceleration
	_acceleration += _steeringForce;
}

void Boid::seekWithingRangeWithMultiplier( ci::Vec3f aTarget, float aMinRange, float aMultiplier = 1.0f )
{
	float rangeSQ = aMinRange * aMinRange;
	float distanceSQ = _position.distanceSquared( aTarget );
	
	// we're as close as we want to get 
	if(distanceSQ < rangeSQ) {
		return;
	}
	
	seekWithMultiplier( aTarget, aMultiplier );
}
void Boid::arriveWithEaseDistanceWithMultiplier( ci::Vec3f aTarget, float anEaseDistance, float aMultiplier = 1.0f )
{
	_steeringForce = steerTowardsWithEaseDistance( aTarget, anEaseDistance );
	_steeringForce *= aMultiplier;
	
	_acceleration += _steeringForce;
}

void Boid::fleeIfWithinDistanceWithMultiplier( ci::Vec3f aTarget, float aMinRange, float aMultiplier )
{
	float panicDistanceSQ = aMinRange * aMinRange;
	float distanceSQ = _position.distanceSquared( aTarget );
	
	// we're far away enough not to care
	if(distanceSQ > panicDistanceSQ) {
		return;
	}
	
	_steeringForce = steerTowardsWithEaseDistance( aTarget, aMinRange );
	_steeringForce *= aMultiplier;
	_steeringForce *= -1;	// Flee is really the same as seek, just inverted
	
	_acceleration += _steeringForce;
}
