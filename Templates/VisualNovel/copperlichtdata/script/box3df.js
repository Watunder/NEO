//+ Nikolaus Gebhardt
// This file is part of the CopperLicht library, copyright by Nikolaus Gebhardt

/**
 * An axis aligned bounding box with a min and a maximal edge.
 * @constructor
 * @class Axis aligned bounding box.
 */
CL3D.Box3d = function()
{
	this.MinEdge = new CL3D.Vect3d();
	this.MaxEdge = new CL3D.Vect3d();
}

/**
 * Minimal Edge of the bounding box
 * @public
 * @type Vect3d
 */
CL3D.Box3d.prototype.MinEdge = null;

/**
 * Maximal Edge of the bounding box
 * @public
 * @type Vect3d
 */
CL3D.Box3d.prototype.MaxEdge = null;

/**
 * Creates a clone of the box
 * @public
 * @returns {CL3D.Box3d} clone
 */
CL3D.Box3d.prototype.clone = function()
{
	var c = new CL3D.Box3d();
	c.MinEdge = this.MinEdge.clone();
	c.MaxEdge = this.MaxEdge.clone();
	return c;
}

/**
 * Returns the center of the box
 * @public
 * @returns {CL3D.Vect3d} center
 */
CL3D.Box3d.prototype.getCenter = function()
{
	var ret = this.MinEdge.add(this.MaxEdge);
	ret.multiplyThisWithScal(0.5);
	return ret;
}

/**
 * Returns the extent (or size) of the box
 * @public
 * @returns {CL3D.Vect3d} extent
 */
CL3D.Box3d.prototype.getExtent = function()
{
	return this.MaxEdge.substract(this.MinEdge);
}

/**
 * Returns all 8 edges of the bounding box
 * @public
 * @returns {Array} edges
 */
CL3D.Box3d.prototype.getEdges = function()
{
	var middle = this.getCenter();
	var diag = middle.substract(this.MaxEdge);
	
	var edges = new Array();
	edges.push(new CL3D.Vect3d(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z));
	edges.push(new CL3D.Vect3d(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z));
	edges.push(new CL3D.Vect3d(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z));
	edges.push(new CL3D.Vect3d(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z));
	edges.push(new CL3D.Vect3d(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z));
	edges.push(new CL3D.Vect3d(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z));
	edges.push(new CL3D.Vect3d(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z));
	edges.push(new CL3D.Vect3d(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z));
	
	return edges;
}

/**
 * Returns if the box intersects with a line
 * @param lineStart {CL3D.Vect3d} start of the line
 * @param lineEnd {CL3D.Vect3d} end of the line
 * @public
 */
CL3D.Box3d.prototype.intersectsWithLine = function(lineStart, lineEnd)
{
	var vect = lineEnd.substract(lineStart);
	var len = vect.getLength();
	
	vect.normalize();			
	
	var middle = lineStart.add(lineEnd).multiplyWithScal(0.5);
	
	return this.intersectsWithLineImpl(middle, vect, len * 0.5);
}

/**
 * @private
 */
CL3D.Box3d.prototype.intersectsWithLineImpl = function(linemiddle, linevect, halflength)
{
	var e = this.getExtent().multiplyWithScal(0.5);
	var t = this.getCenter().substract(linemiddle);

	if ((Math.abs(t.X) > e.X + halflength * Math.abs(linevect.X)) ||
		(Math.abs(t.Y) > e.Y + halflength * Math.abs(linevect.Y)) ||
		(Math.abs(t.Z) > e.Z + halflength * Math.abs(linevect.Z)) )
		return false;

	var r = e.Y * Math.abs(linevect.Z) + e.Z * Math.abs(linevect.Y);
	if (Math.abs(t.Y*linevect.Z - t.Z*linevect.Y) > r )
		return false;

	r = e.X * Math.abs(linevect.Z) + e.Z * Math.abs(linevect.X);
	if (Math.abs(t.Z*linevect.X - t.X*linevect.Z) > r )
		return false;

	r = e.X * Math.abs(linevect.Y) + e.Y * Math.abs(linevect.X);
	if (Math.abs(t.X*linevect.Y - t.Y*linevect.X) > r)
		return false;

	return true;
}

/**
 * Adds a point to the bounding box, increasing the box if the point is outside of the box
  * @public
 */
CL3D.Box3d.prototype.addInternalPoint = function(x, y, z)
{
	if (x>this.MaxEdge.X) this.MaxEdge.X = x;
	if (y>this.MaxEdge.Y) this.MaxEdge.Y = y;
	if (z>this.MaxEdge.Z) this.MaxEdge.Z = z;

	if (x<this.MinEdge.X) this.MinEdge.X = x;
	if (y<this.MinEdge.Y) this.MinEdge.Y = y;
	if (z<this.MinEdge.Z) this.MinEdge.Z = z;
}

/**
 * Adds a point to the bounding box, increasing the box if the point is outside of the box
  * @public
  * @param v {CL3D.Vect3d} 3d vector representing the point
 */
CL3D.Box3d.prototype.addInternalPointByVector = function(v)
{
	this.addInternalPoint(v.X, v.Y, v.Z);
}


/**
 * Adds a box to the bounding box
  * @public
  * @param v {CL3D.Box3d} 3d bounding box to add
 */
CL3D.Box3d.prototype.addInternalBox = function(box)
{
	this.addInternalPointByVector(box.MinEdge);
	this.addInternalPointByVector(box.MaxEdge);
}

/**
 * Returns if the box intersects with another box
 * @param box {CL3D.Box3d} other box
 * @public
 */
CL3D.Box3d.prototype.intersectsWithBox = function(box)
{
	return this.MinEdge.X <= box.MaxEdge.X && this.MinEdge.Y <= box.MaxEdge.Y && this.MinEdge.Z <= box.MaxEdge.Z &&
	       this.MaxEdge.X >= box.MinEdge.X && this.MaxEdge.Y >= box.MinEdge.Y && this.MaxEdge.Z >= box.MinEdge.Z;
}

/**
 * Returns if a point is inside this box
 * @param p {CL3D.Vect3d} point to test
 * @public
 */
CL3D.Box3d.prototype.isPointInside = function(p)
{
	return      p.X >= this.MinEdge.X && p.X <= this.MaxEdge.X &&
				p.Y >= this.MinEdge.Y && p.Y <= this.MaxEdge.Y &&
				p.Z >= this.MinEdge.Z && p.Z <= this.MaxEdge.Z;
}

/**
 * Resets the bounding box
 * @public
 */
CL3D.Box3d.prototype.reset = function(x, y, z)
{
	this.MaxEdge.set(x,y,z);
	this.MinEdge.set(x,y,z);
}

/**
 * Returns true if the box is empty (MinEdge == MaxEdge)
 * @public
 */
CL3D.Box3d.prototype.isEmpty = function()
{
	return this.MaxEdge.equals(this.MinEdge);
}
