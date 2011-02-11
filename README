This is a port of @soulwire 's Boid class from AS3.  
It's not fleshed out yet (no wander / flock), but steering is functional.  

#Basic Usage:
`
//	In header 
	Boid		*_boid;
`

`
// In class
// Creation
	_boidMaxSpeed = 100;
	_boidMaxForce = 10;
	// Create boid
	_boid = new Boid( _boidMaxSpeed, _boidMaxForce );
	_boid->setPosition( _target.x, _target.y, _target.z );
	
// Update loop
	_boid->seekWithingRangeWithMultiplier( ci::Vec2f( getMousePos().x, getMousePos.y), 25.0f, 1.0f );
`