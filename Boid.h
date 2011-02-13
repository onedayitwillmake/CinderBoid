/*
 *  Boid.h
 *  A hybrid port of SoulWire's version (AS3) of CraigReynolds boids (C++)
 *
 *  Created by Mario Gonzalez on 2/9/11.
 *  Copyright 2011 Whale Island Games. All rights reserved.
 */
// And of course, obviously credit to Craig Reynolds
// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------

#include "cinder/Vector.h"
#define BOID_EPSILON	0.001f

namespace onedayitwillmake
{
	class Boid
	{	
	public:
		Boid();
		Boid( float aMaxSpeed, float aMaxForce );
		
		ci::Vec3f	_velocity;
		ci::Vec3f	_acceleration;
		ci::Vec3f	_steeringForce;
		
		// State
		ci::Vec3f	_position;
		ci::Vec3f	_previousPosition;
		float		_radius;
		
		// Wander
		float		_wanderRadius;
		float		_wanderLookAheadDistance;
		float		_wanderMaxTurningSpeed;
		
		
		void		update();
		void		handleBorder();
		
	// Accessors
		void		setMaxSpeed(float aSpeed) { _maxSpeed = aSpeed; _maxSpeedSquared = _maxSpeed * _maxSpeed;};
		void		setMaxSteeringForce( float aMaxSteeringForce ) { _maxForce = aMaxSteeringForce; _maxForceSquared = _maxForce*_maxForce; };
		
		// Wander properties
		void		setWanderRadius( float aWanderRadius ) { _wanderRadius = aWanderRadius; }; // How big a circle to pick a random point from
		void		setWanderLookAheadDistance( float aLookAheadDistance ) { _wanderLookAheadDistance = aLookAheadDistance; }; // How far ahead to create circle above
		void		setWanderMaxTurningSpeed( float aMaxTurningSpeed ) { _wanderMaxTurningSpeed = aMaxTurningSpeed; }; // How much we can turn left or right per tick
		
		void		setPosition(float tX, float tY, float tZ) { _position.x = tX; _position.y = tY; _position.z = tZ; };
		ci::Vec3f	getPosition() { return _position; };
		
	//	Movement
		void		applyBreakingForce( float aBrakingForce );
		void		wander( float aMultiplier );
		// Seeking
		void		seek( ci::Vec3f aTarget, float aMultiplier );
		void		seekWithingRange( ci::Vec3f aTarget, float aMinRange, float aMultiplier );
		void		arriveWithEaseDistance( ci::Vec3f aTarget, float anEaseDistance, float aMultiplier );
		// Avoid
		void		fleeIfWithinDistance( ci::Vec3f aTarget, float aMinRange, float aMultiplier );
		
		// Flocking
	//	void		flock
	private:
		float	_maxForce;
		float	_maxForceSquared;
		
		float	_maxSpeed;
		float	_maxSpeedSquared;
		
		bool	_doRotation;
		
		float	_wanderTheta;	// Modified incrimentally each frmae
		
		// Steering
		ci::Vec3f	steerTowardsWithEaseDistance( ci::Vec3f aTarget, float anEaseDistance );
		
		// DEBUG
//		void		drawBasic3dSphericalVehicle ();
	};
}