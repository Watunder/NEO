//+ Nikolaus Gebhardt
// This file is part of the CopperLicht library, copyright by Nikolaus Gebhardt
// This file is part of the CopperLicht engine, (c) by N.Gebhardt

/**
 * A 3d vertex, ususally used in {@link MeshBuffer}s
 * @constructor
 * @class A 3d vertex, ususally used in {@link MeshBuffer}s
 * @param {Boolean} init set to true to let the vertex members (Position, Normal etc) be initialized with instances of classes, false if not.
 */
CL3D.Vertex3D = function(init)
{
	if (init)
	{
		this.Pos = new CL3D.Vect3d();
		this.Normal = new CL3D.Vect3d();
		this.Color = 0xff404040; //0xffffffff;
		this.TCoords = new CL3D.Vect2d();
		this.TCoords2 = new CL3D.Vect2d();
	}
}

/** 
 * 3D Position of the vertex
 * @public
 * @type Vertex3d
 */
CL3D.Vertex3D.prototype.Pos = null;

/** 
 * Normal of the vertex
 * @public
 * @type Vertex3d
 */
CL3D.Vertex3D.prototype.Normal = null;

/** 
 * Color of the vertex
 * @public
 * @type int
 */
CL3D.Vertex3D.prototype.Color = 0;

/** 
 * Texture coordinate 1 of the vertex
 * @public
 * @type Vertex3d
 */
CL3D.Vertex3D.prototype.TCoords = null;

/** 
 * Texture coordinate 2 of the vertex
 * @public
 * @type Vertex3d
 */
CL3D.Vertex3D.prototype.TCoords2 = null;


CL3D.cloneVertex3D = function(c)
{
	var r = new CL3D.Vertex3D();
	r.Pos = c.Pos.clone();
	r.Color = c.Color;
	r.Normal = c.Normal.clone();
	r.TCoords = c.TCoords.clone();
	r.TCoords2 = c.TCoords2.clone();
	return r;
}

/**
 * @private
 */
CL3D.createVertex = function(x, y, z, nx, ny, nz, clr, s, t)
{
	var vtx = new CL3D.Vertex3D(true);
	vtx.Pos.X = x;
	vtx.Pos.Y = y;
	vtx.Pos.Z = z;
	vtx.Normal.X = nx;
	vtx.Normal.Y = ny;
	vtx.Normal.Z = nz;
	vtx.Color = clr;
	vtx.TCoords.X = s;
	vtx.TCoords.Y = t;
	vtx.TCoords2.X = s;
	vtx.TCoords2.Y = t;
	return vtx;
}