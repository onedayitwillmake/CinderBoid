/*
 *  Boid.h
 *  A hybrid port of SoulWire's version (AS3) of CraigReynolds boids (C++)
 *
 *  Created by Mario Gonzalez on 2/9/11.
 *  Copyright 2011 Whale Island Games. All rights reserved.
 */

#include "Boid.h"
#include "cinder/Rand.h"
#include "cinder/Color.h"

namespace onedayitwillmake 
{
	Boid::Boid() 
	{
		_velocity = ci::Rand::randVec3f();
	};
	
	Boid::Boid( float aMaxSpeed, float aMaxSteeringForce )
	{
		setMaxSpeed( aMaxSpeed );
		setMaxSteeringForce( aMaxSteeringForce );
		_velocity = ci::Rand::randVec3f();
	}

#pragma mark Tick
	void Boid::update()
	{
		_previousPosition.x = _position.x;
		_previousPosition.y = _position.y;
		_previousPosition.z = _position.z;

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

#pragma mark Seek
	void Boid::seek( ci::Vec3f aTarget, float aMultiplier = 1.0f )
	{
		// Apply steering force
		_steeringForce = steerTowardsWithEaseDistance( aTarget );
		_steeringForce *= aMultiplier;
		
		// Append to acceleration
		_acceleration += _steeringForce;
	}

	void Boid::seekWithingRange( ci::Vec3f aTarget, float aMinRange, float aMultiplier = 1.0f )
	{
		float rangeSQ = aMinRange * aMinRange;
		float distanceSQ = _position.distanceSquared( aTarget );
		
		// we're as close as we want to get 
		if(distanceSQ < rangeSQ) {
			return;
		}
		
		seek( aTarget, aMultiplier );
	}
	void Boid::arriveWithEaseDistance( ci::Vec3f aTarget, float anEaseDistance, float aMultiplier = 1.0f )
	{
		_steeringForce = steerTowardsWithEaseDistance( aTarget, anEaseDistance );
		_steeringForce *= aMultiplier;
		
		_acceleration += _steeringForce;
	}
	
#pragma mark Flee
	void Boid::fleeIfWithinDistance( ci::Vec3f aTarget, float aMinRange, float aMultiplier = 1.0f)
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
	
#pragma mark Wander
	void Boid::wander( float aMultiplier = 1.0f )
	{
		// Modify _wander
		//Theta randomly
		_wanderTheta += ci::Rand::randFloat(-_wanderMaxTurningSpeed, _wanderMaxTurningSpeed);
		
		// Add our speed to where we are, plus wanderDistance ( How far in front of where we are, we project ourselves when wandering)
		// This is a straight line in front of us
		ci::Vec3f futurePosition = _velocity.safeNormalized();
		futurePosition *= _wanderLookAheadDistance;
		futurePosition += _position;
		
		// Move left or right a little
		ci::Vec3f offset = ci::Vec3f( cosf(_wanderTheta), sinf(_wanderTheta), tanf( _wanderTheta ) );
		offset *= _wanderRadius;
		
		// Steer towards futureposition (a straight line in front of us), and offset (left or right from that point)
		ci::Vec3f newTarget = futurePosition + offset;
		_steeringForce = steerTowardsWithEaseDistance(newTarget);
		_steeringForce *= aMultiplier;
		
		_acceleration += _steeringForce;
	}
	
#pragma mark Debug
	// From OpenSteer
	void Boid::drawBasic3dSphericalVehicle ()
	{
	//	ci::Color color = ci::Color( 0.25, 0.25, 0.4 );
//		//const Color& color
//		// "aspect ratio" of body (as seen from above)
//		const float x = 0.5f;
//		const float y = sqrtf(1 - (x * x));
//		
//		float radius = 5;
//		// radius and position of vehicle
//		const float r = radius;//vehicle.radius();
//		const ci::Vec3f& p = _position;//vehicle.position();
//		
//		// FakeFoward
//		ci::Vec3f fakeForward	= _velocity.normalized();
//		ci::Vec3f fakeSide		= fakeForward + ci::Vec3f::xAxis();
//		ci::Vec3f fakeUp		= fakeForward + ci::Vec3f::yAxis();
//		
//		
//		// body shape parameters
//		const ci::Vec3f f = r * fakeForward;
//		const ci::Vec3f s = r * fakeSide * x;
//		const ci::Vec3f u = r * fakeUp * x * 0.5f;
//		const ci::Vec3f b = r * fakeForward * -y;
//		
//		// vertex positions
//		const ci::Vec3f nose   = p + f;
//		const ci::Vec3f side1  = p + b - s;
//		const ci::Vec3f side2  = p + b + s;
//		const ci::Vec3f top    = p + b + u;
//		const ci::Vec3f bottom = p + b - u;
//		
//		// colors
//		const float j = +0.05f;
//		const float k = -0.05f;
//		const ci::Color color1 = color + ci::Color(j, j, k);
//		const ci::Color color2 = color + ci::Color(j, k, j);
//		const ci::Color color3 = color + ci::Color(k, j, j);
//		const ci::Color color4 = color + ci::Color(k, j, k);
//		const ci::Color color5 = color + ci::Color(k, k, j);
//		
//		ci::gl::drawCube();
//		// draw body
//		iDrawTriangle (nose,  side1,  top,    color1);  // top, side 1
//		iDrawTriangle (nose,  top,    side2,  color2);  // top, side 2
//		iDrawTriangle (nose,  bottom, side1,  color3);  // bottom, side 1
//		iDrawTriangle (nose,  side2,  bottom, color4);  // bottom, side 2
//		iDrawTriangle (side1, side2,  top,    color5);  // top back
//		iDrawTriangle (side2, side1,  bottom, color5);  // bottom back
	}
}