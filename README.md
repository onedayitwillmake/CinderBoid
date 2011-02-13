This is a port of @soulwire 's Boid class from AS3.
It's not fleshed out yet ( currenly only contains seek / flee / wander ), but steering is functional

#Basic Usage
### .cpp
#### Creation
	float boidMaxSpeed = 100;
	float boidMaxForce = 25;

    // Create
	_boid = new Boid( boidMaxSpeed, boidMaxForce );
	_boid->setPosition( _target.x, _target.y, _target.z );
    
    // Set wander properties
	_boid->setWanderRadius( 75.0f );
	_boid->setWanderMaxTurningSpeed( M_PI / 32 ); // 
	_boid->setWanderLookAheadDistance( 300 );

	
#### Update
    _boid->wander( 0.3f );
    _boid->seek( _mousePositionXYZ, 0.8f );
    _boid->update();