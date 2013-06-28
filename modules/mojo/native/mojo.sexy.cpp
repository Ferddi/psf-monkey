
// Sexy mojo runtime.
//
// No warranty implied; use at your own risk.

//***** gxtkGraphics.h *****

class gxtkSurface;

class gxtkGraphics : public Object
{
public:

	enum{
		MAX_VERTS=1024,
		MAX_POINTS=MAX_VERTS,
		MAX_LINES=(MAX_VERTS/2),
		MAX_QUADS=(MAX_VERTS/4)
	};

	int blend;
	int colorARGB;
	float r,g,b,alpha;
	float ix,iy,jx,jy,tx,ty;
	bool tformed;

	Point vertices[MAX_VERTS*5];
	unsigned short quadIndices[MAX_QUADS*6];
	TriVertex triVertices[MAX_VERTS];

	int primType;
	int primCount;
	gxtkSurface *primSurf;
	
	int scissorX1;
	int scissorY1;
	int scissorX2;
	int scissorY2;
	
	gxtkGraphics();
	
	void Flush();
	
	//***** GXTK API *****
	virtual int Width();
	virtual int Height();
	
	virtual int  BeginRender();
	virtual void EndRender();
	virtual void DiscardGraphics();

	virtual gxtkSurface *LoadSurface( String path );
	virtual gxtkSurface *LoadSurface__UNSAFE__( gxtkSurface *surface,String path );
	virtual gxtkSurface *CreateSurface( int width,int height );
	
	virtual int Cls( float r,float g,float b );
	virtual int SetAlpha( float alpha );
	virtual int SetARGB();
	virtual int SetColor( float r,float g,float b );
	virtual int SetBlend( int blend );
	virtual int SetScissor( int x,int y,int w,int h );
	virtual int SetMatrix( float ix,float iy,float jx,float jy,float tx,float ty );
	
	virtual int DrawPoint( float x,float y );
	virtual int DrawRect( float x,float y,float w,float h );
	virtual int DrawLine( float x1,float y1,float x2,float y2 );
	virtual int DrawOval( float x1,float y1,float x2,float y2 );
	virtual int DrawPoly( Array<float> verts );
	virtual int DrawPoly2( Array<Float> verts,gxtkSurface *surface,int srcx,int srcy );
	virtual int DrawSurface( gxtkSurface *surface,float x,float y );
	virtual int DrawSurface2( gxtkSurface *surface,float x,float y,int srcx,int srcy,int srcw,int srch );
	
	virtual int ReadPixels( Array<int> pixels,int x,int y,int width,int height,int offset,int pitch );
	virtual int WritePixels2( gxtkSurface *surface,Array<int> pixels,int x,int y,int width,int height,int offset,int pitch );
};

class gxtkSurface : public Object
{
public:
	Image *data;
	int width;
	int height;
	int depth;
	float uscale;
	float vscale;
	
	gxtkSurface();
	gxtkSurface(Image *data,int width,int height,int depth );
	
	void SetData(Image *data,int width,int height,int depth );
	
	~gxtkSurface();
	
	//***** GXTK API *****
	virtual int Discard();
	virtual int Width();
	virtual int Height();
	virtual int Loaded();
	virtual bool OnUnsafeLoadComplete();
};

//***** gxtkGraphics.cpp *****

static int Pow2Size( int n )
{
	int i=1;
	while( i<n ) i+=i;
	return i;
}

gxtkGraphics::gxtkGraphics()
{
	for( int i=0;i<MAX_QUADS;++i )
	{
		quadIndices[i*6  ]=(short)(i*4);
		quadIndices[i*6+1]=(short)(i*4+1);
		quadIndices[i*6+2]=(short)(i*4+2);
		quadIndices[i*6+3]=(short)(i*4);
		quadIndices[i*6+4]=(short)(i*4+2);
		quadIndices[i*6+5]=(short)(i*4+3);
	}

	primType = 0;
	primCount = 0;

	scissorX1 = 0;
	scissorY1 = 0;
	scissorX2 = Width();
	scissorY2 = Height();
}

void gxtkGraphics::Flush()
{
	if( !primCount ) return;
		
	switch( primType )
	{
	case 1:
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->DrawPoints(triVertices, primCount);
		}
		break;
	}

	primCount=0;
}

//***** GXTK API *****

int gxtkGraphics::Width()
{
	if (gPopCapApp != NULL)
	{
		return gPopCapApp->mWidth;
	}

	return 0;
}

int gxtkGraphics::Height()
{
	if (gPopCapApp != NULL)
	{
		return gPopCapApp->mHeight;
	}

	return 0;
}

int gxtkGraphics::BeginRender()
{
	return 1;
}

void gxtkGraphics::EndRender()
{
	Flush();
}

void gxtkGraphics::DiscardGraphics()
{
}

int gxtkGraphics::Cls( float r,float g,float b )
{
	Color color;

	if (gPopCapGraphics != NULL)
	{
		gPopCapGraphics->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		color = gPopCapGraphics->GetColor();	
		gPopCapGraphics->SetColor(Color((int)r, (int)g, (int)b));
		gPopCapGraphics->FillRect(0, 0, gPopCapApp->mWidth, gPopCapApp->mHeight);
		gPopCapGraphics->SetColor(color);
	}
	SetBlend(blend);

	return 0;
}

int gxtkGraphics::SetARGB()
{
	int a = int(this->alpha * 255);
	// Popcap Sexy Framework when colorARGB is 0, it actually draw the image fully.
	// So when you are doing alpha fading, it will fade and then pop up at the very end!
	// This if statement is a hack so when it is 0 set it to 1, so it is really dark, but not fully drawn.
	if (a <= 0) a = 1;

	int red = 0;
	int grn = 0;
	int blu = 0;
	
	// Ferdi's comment: look at D3DInterface::SetupDrawMode.  Popcap made a fix for the color
	// to be multiplied by the alpha, but for some reason they comment it out.  So this is a 
	// hack for that fix.  If it is 3D and it is an additive blend then ...
	if (gIs3D == true)
	{
		if (this->blend == 1)
		{
			red = (int)this->r * alpha;
			grn = (int)this->g * alpha;
			blu = (int)this->b * alpha;
		}
		else
		{
			red = (int)this->r;
			grn = (int)this->g;
			blu = (int)this->b;
		}
	}
	else
	{
		//if (this->blend == 1)
		//{
		//	red = (int)this->r * alpha;
		//	grn = (int)this->g * alpha;
		//	blu = (int)this->b * alpha;
		//}
		//else
		{
			red = (int)this->r;
			grn = (int)this->g;
			blu = (int)this->b;
		}
	}
	
	colorARGB = (a << 24) | (red << 16) | (grn << 8) | blu;
	if (gPopCapGraphics != NULL)
	{
		gPopCapGraphics->SetColor(Color(red, grn, blu, a));
	}

	return 0;
}

int gxtkGraphics::SetAlpha( float alpha )
{
	this->alpha=alpha;

	SetARGB();

	return 0;
}

int gxtkGraphics::SetColor( float r,float g,float b )
{
	this->r=r;
	this->g=g;
	this->b=b;

	SetARGB();

	return 0;
}

int gxtkGraphics::SetBlend( int blend )
{
	Flush();
	
	switch( blend )
	{
	case 1:
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		}
		break;
	default:
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		}
	}
	this->blend = blend;

	SetARGB();

	return 0;
}

int gxtkGraphics::SetScissor( int x,int y,int w,int h )
{
	Flush();
	
	if( x!=0 || y!=0 || w!=Width() || h!=Height() )
	{
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->SetClipRect(x, y, w, h);
		}

		scissorX1 = x;
		scissorY1 = y;
		scissorX2 = x + w;
		scissorY2 = y + h;
	}
	else
	{
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->ClearClipRect();
		}

		scissorX1 = 0;
		scissorY1 = 0;
		scissorX2 = Width();
		scissorY2 = Height();
	}

	return 0;
}

int gxtkGraphics::SetMatrix( float ix,float iy,float jx,float jy,float tx,float ty )
{
	tformed=(ix!=1 || iy!=0 || jx!=0 || jy!=1 || tx!=0 || ty!=0);

	this->ix=ix;this->iy=iy;this->jx=jx;this->jy=jy;this->tx=tx;this->ty=ty;

	return 0;
}

int gxtkGraphics::DrawLine( float x0,float y0,float x1,float y1 )
{
	if( primType!=2 || primCount==MAX_LINES || primSurf )
	{
		Flush();
		primType=2;
		primSurf=0;
	}

	if (tformed)
	{
		float tx0=x0,tx1=x1;
		x0=tx0 * ix + y0 * jx + tx;y0=tx0 * iy + y0 * jy + ty;
		x1=tx1 * ix + y1 * jx + tx;y1=tx1 * iy + y1 * jy + ty;
	}

	if (gPopCapGraphics != NULL)
	{
		gPopCapGraphics->DrawLine((int)x0, (int)y0, (int)x1, (int)y1);
	}
	
	return 0;
}

int gxtkGraphics::DrawPoint( float x,float y )
{
	if( primType!=1 || primCount==MAX_POINTS || primSurf )
	{
		Flush();
		primType=1;
		primSurf=0;
	}
	
	if( tformed )
	{
		float px=x;
		x=px * ix + y * jx + tx;
		y=px * iy + y * jy + ty;
	}

	if (x >= scissorX1 && x < scissorX2 && y >= scissorY1 && y < scissorY2)
	{
		TriVertex *vp = &triVertices[primCount++];
		vp->x = x;
		vp->y = y;
		vp->color = colorARGB;
	}

	return 0;	
}
	
int gxtkGraphics::DrawRect( float x,float y,float w,float h )
{
	if( primType!=4 || primCount==MAX_QUADS || primSurf )
	{
		Flush();
		primType=4;
		primSurf=0;
	}

	if( tformed )
	{
		float x0=x,x1=x+w,x2=x+w,x3=x;
		float y0=y,y1=y,y2=y+h,y3=y+h;

		float tx0=x0,tx1=x1,tx2=x2,tx3=x3;
		
		vertices[0].mX = (int)(tx0 * ix + y0 * jx + tx);
		vertices[0].mY = (int)(tx0 * iy + y0 * jy + ty);

		vertices[1].mX = (int)(tx1 * ix + y1 * jx + tx);
		vertices[1].mY = (int)(tx1 * iy + y1 * jy + ty);

		vertices[2].mX = (int)(tx2 * ix + y2 * jx + tx);
		vertices[2].mY = (int)(tx2 * iy + y2 * jy + ty);

		vertices[3].mX = (int)(tx3 * ix + y3 * jx + tx);
		vertices[3].mY = (int)(tx3 * iy + y3 * jy + ty);
		
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->PolyFill(vertices, 4, true);
		}
	}
	else
	{
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->FillRect((int)x, (int)y, (int)w, (int)h);
		}
	}
	
	return 0;
}

int gxtkGraphics::DrawOval( float x,float y,float w,float h )
{
	Flush();
	primType=5;
	primSurf=0;
	
	float xr = w / 2.0f;
	float yr = h / 2.0f;

	int segs;
	
	if( tformed )
	{
		float dx_x=xr * ix;
		float dx_y=xr * iy;
		float dx=sqrtf( dx_x*dx_x+dx_y*dx_y );
		float dy_x=yr * jx;
		float dy_y=yr * jy;
		float dy=sqrtf( dy_x*dy_x+dy_y*dy_y );
		segs=(int)( dx+dy );
	}
	else
	{
		segs=(int)( abs( xr )+abs( yr ) );
	}
	
	if( segs<12 )
	{
		segs = 12;
	}
	else if( segs>MAX_VERTS )
	{
		segs = MAX_VERTS;
	}
	else
	{
		segs&=~3;
	}

	float x0=x+xr,y0=y+yr;

	for( int i=0; i<segs; ++i )
	{
		float th=i * 6.28318531f / segs;

		float px=x0+cosf( th ) * xr;
		float py=y0-sinf( th ) * yr;
		
		if( tformed )
		{
			float ppx=px;
			px=ppx * ix + py * jx + tx;
			py=ppx * iy + py * jy + ty;
		}
		
		vertices[i].mX = (int)px;
		vertices[i].mY = (int)py;
	}

	if (gPopCapGraphics != NULL)
	{
		gPopCapGraphics->PolyFill(vertices, segs, true);
	}
	
	return 0;
}

int gxtkGraphics::DrawPoly( Array<float> verts )
{
	int n = verts.Length() / 2;
	if (n<3 || n>MAX_VERTS) return 0;
	
	Flush();
	primType=5;
	primSurf=0;
	
	for (int i=0; i<n; ++i)
	{
		float px=verts[i*2];
		float py=verts[i*2+1];
		
		if( tformed )
		{
			float ppx=px;
			px=ppx * ix + py * jx + tx;
			py=ppx * iy + py * jy + ty;
		}
		
		vertices[i].mX = (int)px;
		vertices[i].mY = (int)py;
	}

	if (gPopCapGraphics != NULL)
	{
		gPopCapGraphics->PolyFill(vertices, n, true);
	}

	return 0;
}

int gxtkGraphics::DrawPoly2( Array<Float> verts,gxtkSurface *surface,int srcx,int srcy )
{
	int n=verts.Length()/4;
	if( n<1 || n>MAX_VERTS ) return 0;
		
	Flush();
	primType=5;
	primSurf=surface;

	TriVertex *vp = &triVertices[0];

	for( int i=0; i < n; ++ i )
	{
		int j = i * 4;

		if( tformed )
		{
			vp->x = verts[j] * ix + verts[j+1] * jx + tx;
			vp->y = verts[j] * iy + verts[j+1] * jy + ty;
		}
		else
		{
			vp->x = verts[j];
			vp->y = verts[j+1];
		}

		vp->u = (srcx + verts[j+2]) * surface->uscale / surface->width;
		vp->v = (srcy + verts[j+3]) * surface->vscale / surface->height;

		vp->color = colorARGB;
		
		vp ++;
	}
	
	for (int i = 2 ; i < n ; i ++ )
	{
		// Draw triangle using fan shape ... 
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->DrawTriangleTex(surface->data, triVertices[0], triVertices[i - 1], triVertices[i]);
		}
	}
	
	return 0;
}


//int rot = 0;

int gxtkGraphics::DrawSurface(gxtkSurface *surface, float x, float y)
{
	if( primType!=4 || primCount==MAX_QUADS || surface!=primSurf )
	{
		Flush();
		primType=4;
		primSurf=surface;
	}

	if (!tformed)
	{
		if (gIs3D)
		{
			if (gPopCapGraphics != NULL)
			{
				gPopCapGraphics->DrawImageF(surface->data, x, y);
			}
		}
		else
		{
			if (gPopCapGraphics != NULL)
			{
				gPopCapGraphics->DrawImage(surface->data, (int)x, (int)y);
			}
		}
	}
	else
	{
		float w=(float)surface->Width();
		float h=(float)surface->Height();
		float x0=x,x1=x+w,x2=x+w,x3=x;
		float y0=y,y1=y,y2=y+h,y3=y+h;
		float u0=0,u1=1.0; //*surf->uscale;
		float v0=0,v1=1.0; //*surf->vscale;

		if( tformed )
		{
			float tx0=x0,tx1=x1,tx2=x2,tx3=x3;
			x0=tx0 * ix + y0 * jx + tx;y0=tx0 * iy + y0 * jy + ty;
			x1=tx1 * ix + y1 * jx + tx;y1=tx1 * iy + y1 * jy + ty;
			x2=tx2 * ix + y2 * jx + tx;y2=tx2 * iy + y2 * jy + ty;
			x3=tx3 * ix + y3 * jx + tx;y3=tx3 * iy + y3 * jy + ty;
		}
		
		TriVertex vp1, vp2, vp3, vp4;
		
		vp1.x = x0; vp1.y = y0; vp1.u = u0; vp1.v = v0; vp1.color = colorARGB;
		vp2.x = x1; vp2.y = y1; vp2.u = u1; vp2.v = v0; vp2.color = colorARGB;
		vp3.x = x2; vp3.y = y2; vp3.u = u1; vp3.v = v1; vp3.color = colorARGB;
		vp4.x = x3; vp4.y = y3; vp4.u = u0; vp4.v = v1; vp4.color = colorARGB;
		
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->DrawTriangleTex(surface->data, vp1, vp2, vp3);
			gPopCapGraphics->DrawTriangleTex(surface->data, vp1, vp3, vp4);
		}
	}
	
	return 0;
}

int gxtkGraphics::DrawSurface2( gxtkSurface *surface,float x,float y,int srcx,int srcy,int srcw,int srch )
{
	if( primType!=4 || primCount==MAX_QUADS || surface!=primSurf )
	{
		Flush();
		primType=4;
		primSurf=surface;
	}
	
	if (!tformed)
	{
		if (gIs3D)
		{
			if (gPopCapGraphics != NULL)
			{
				gPopCapGraphics->DrawImageF(surface->data, x, y, Rect(srcx, srcy, srcw, srch));
			}
		}
		else
		{
			if (gPopCapGraphics != NULL)
			{
				gPopCapGraphics->DrawImage(surface->data, (int)x, (int)y, Rect(srcx, srcy, srcw, srch));
			}
		}
	}
  else
	{
		float w=(float)surface->Width();
		float h=(float)surface->Height();
		float u0=srcx/w,u1=(srcx+srcw)/w;
		float v0=srcy/h,v1=(srcy+srch)/h;
		w=(float)srcw;
		h=(float)srch;
		float x0=x,x1=x+w,x2=x+w,x3=x;
		float y0=y,y1=y,y2=y+h,y3=y+h;

		if( tformed )
		{
			float tx0=x0,tx1=x1,tx2=x2,tx3=x3;
			x0=tx0 * ix + y0 * jx + tx;y0=tx0 * iy + y0 * jy + ty;
			x1=tx1 * ix + y1 * jx + tx;y1=tx1 * iy + y1 * jy + ty;
			x2=tx2 * ix + y2 * jx + tx;y2=tx2 * iy + y2 * jy + ty;
			x3=tx3 * ix + y3 * jx + tx;y3=tx3 * iy + y3 * jy + ty;
		}
		
		TriVertex vp1;
		TriVertex vp2;
		TriVertex vp3;
		TriVertex vp4;
		
		vp1.x = x0; vp1.y = y0; vp1.u = u0; vp1.v = v0; vp1.color = colorARGB;
		vp2.x = x1; vp2.y = y1; vp2.u = u1; vp2.v = v0; vp2.color = colorARGB;
		vp3.x = x2; vp3.y = y2; vp3.u = u1; vp3.v = v1; vp3.color = colorARGB;
		vp4.x = x3; vp4.y = y3; vp4.u = u0; vp4.v = v1; vp4.color = colorARGB;
		
		if (gPopCapGraphics != NULL)
		{
			gPopCapGraphics->DrawTriangleTex(surface->data, vp1, vp2, vp3);
			gPopCapGraphics->DrawTriangleTex(surface->data, vp1, vp3, vp4);
		}
	}
	
	return 0;
}
	
int gxtkGraphics::ReadPixels( Array<int> pixels,int x,int y,int width,int height,int offset,int pitch )
{
	Flush();

	int w = gPopCapApp->mDDInterface->mWidth;
	unsigned long *p = gPopCapApp->mDDInterface->PrimarySurfaceGetBits();

	if (p != NULL)
	{
		for( int py=0;py<height;++py )
		{
			memcpy( &pixels[offset+py*pitch],&p[py*w],width*4 );
		}
		
		delete p;
	}
	
	return 0;
}

int gxtkGraphics::WritePixels2( gxtkSurface *surface,Array<int> pixels,int x,int y,int width,int height,int offset,int pitch )
{
	Flush();

	ulong * p = new ulong[width * height + 1];

	if (p != NULL)
	{
		ulong * d = p;
		
		for (int py = 0; py < height; ++ py)
		{
			ulong * s = (unsigned long *)&pixels[offset + py * pitch];
			for(int px = 0; px < width; ++ px)
			{
				*d ++ = *s ++;
			}
		}
		
		((DDImage * )surface->data)->SetBits(p, surface->width, surface->height, true);
		
		delete p;
	}

	return 0;
}

//***** gxtkSurface *****

gxtkSurface::gxtkSurface():
data(0),width(0),height(0),depth(0),uscale(1.0),vscale(1.0){
}

gxtkSurface::gxtkSurface( Image *data,int width,int height,int depth ):
data(data),width(width),height(height),depth(depth),uscale(1.0),vscale(1.0)
{
}

void gxtkSurface::SetData( Image *data,int width,int height,int depth )
{
	this->data=data;
	this->width=width;
	this->height=height;
	this->depth=depth;
}

gxtkSurface::~gxtkSurface()
{
	Discard();
}

int gxtkSurface::Discard()
{
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return 0;
}

int gxtkSurface::Width()
{
	return width;
}

int gxtkSurface::Height()
{
	return height;
}

int gxtkSurface::Loaded()
{
	return 1;
}

bool gxtkSurface::OnUnsafeLoadComplete()
{
	return true;
}

gxtkSurface *gxtkGraphics::LoadSurface__UNSAFE__( gxtkSurface *surface,String path )
{
	int monkeyPos = path.Find("monkey://");
	if (monkeyPos >= 0)
	{
		monkeyPos += 9;
	}
	else
	{
		monkeyPos = 0;
	}
	
	int aLastDotPos = path.FindLast(".");
	int aLastSlashPos = max((int)path.FindLast("\\"), (int)path.FindLast("/"));

	const Char * p1 = path.Data();
	char p2[256];

	if (aLastDotPos > aLastSlashPos)
	{
		int i = 0;

		for (i = monkeyPos ; i < aLastDotPos ; i ++ )
		{
			p2[i - monkeyPos] = (char)p1[i];
		}
		p2[i - monkeyPos] = 0;
	}

	Image *data = (DDImage *)gPopCapApp->GetImage(p2);
	if (data == NULL)
	{
		return 0;
	}

	((DDImage*)data)->Palletize();
	
	surface->width = data->mWidth;
	surface->height = data->mHeight;
	surface->data = data;
	
	surface->depth = 0; // ?? not possible in SexyFramework

	return surface;
}

gxtkSurface *gxtkGraphics::LoadSurface( String path )
{
	gxtkSurface * surf = LoadSurface__UNSAFE__( new gxtkSurface(), path );

	return surf;
}

gxtkSurface *gxtkGraphics::CreateSurface(int width, int height)
{
	Image *data = (DDImage *)gPopCapApp->CreateImage();
	if (data == NULL)
	{
		return 0;
	}

	gxtkSurface *surf = new gxtkSurface(data, width, height, 4);

	return surf;
}

//***** gxtkAudio.h *****

class gxtkSample;

class gxtkAudio : public Object
{
public:
	float channelVolume[33];
	int channelFlags[33];

	gxtkAudio();

	virtual void mark();

	//***** GXTK API *****
	virtual int Suspend();
	virtual int Resume();

	virtual gxtkSample *LoadSample__UNSAFE__( gxtkSample *sample,String path );
	virtual gxtkSample *LoadSample( String path );
	virtual int PlaySample( gxtkSample *sample,int channel,int flags );

	virtual int StopChannel( int channel );
	virtual int PauseChannel( int channel );
	virtual int ResumeChannel( int channel );
	virtual int ChannelState( int channel );
	virtual int SetVolume( int channel,float volume );
	virtual int SetChannelVolume(int channel);
	virtual int SetPan( int channel,float pan );
	virtual int SetRate( int channel,float rate );
	
	virtual int PlayMusic( String path,int flags );
	virtual int StopMusic();
	virtual int PauseMusic();
	virtual int ResumeMusic();
	virtual int MusicState();
	virtual int SetMusicVolume( float volume );
};

class gxtkSample : public Object
{
public:
	int id;

	gxtkSample();
	gxtkSample( int buf );
	~gxtkSample();
	
	void SetBuffer( int buf );
	
	//***** GXTK API *****
	virtual int Discard();
};

//***** gxtkAudio.cpp *****

gxtkAudio::gxtkAudio()
{
	for (int channel = 0 ; channel < 33 ; channel ++)
	{
		channelVolume[channel] = 1.0;
	}
}

void gxtkAudio::mark()
{
}

int gxtkAudio::Suspend()
{
	for( int channel = 0; channel < 33; ++ channel )
	{
		SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

		if (soundInstance != NULL)
		{
			if (soundInstance->IsPlaying() == true)
			{
				soundInstance->Pause();
			}
		}
	}

	return 0;
}

int gxtkAudio::Resume()
{
	for( int channel = 0; channel < 33; ++ channel )
	{
		SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

		if (soundInstance != NULL)
		{
			if (soundInstance->IsPaused() == true)
			{
				if (channelFlags[channel] == 1)
					soundInstance->Play(true, false);
				else
					soundInstance->Play(false, false);
			}
		}
	}

	return 0;
}

int gSoundId = 1;

gxtkSample *gxtkAudio::LoadSample__UNSAFE__( gxtkSample *sample,String path )
{
	int monkeyPos = path.Find("monkey://");
	if (monkeyPos >= 0)
	{
		monkeyPos += 9;
	}
	else
	{
		monkeyPos = 0;
	}
	
	int aLastDotPos = path.FindLast(".");
	int aLastSlashPos = max((int)path.FindLast("\\"), (int)path.FindLast("/"));

	const Char * p1 = path.Data();
	char p2[256];

	if (aLastDotPos > aLastSlashPos)
	{
		int i = 0;

		for (i = monkeyPos ; i < aLastDotPos ; i ++ )
		{
			p2[i - monkeyPos] = (char)p1[i];
		}
		p2[i - monkeyPos] = 0;
	}

	if (!gPopCapApp->mSoundManager->LoadSound(gSoundId, p2))
	{
		return 0;
	}
	
	sample->id = gSoundId;

	gSoundId ++;

	return sample;
}

gxtkSample *gxtkAudio::LoadSample( String path )
{
	return LoadSample__UNSAFE__( new gxtkSample(), path );
}

int gxtkAudio::PlaySample( gxtkSample *sample,int channel,int flags )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel, sample->id);

	if (soundInstance != NULL)
	{
		if (flags == 1)
			soundInstance->Play(true, false);
		else
			soundInstance->Play(false, false);
		SetChannelVolume(channel);
		channelFlags[channel] = flags;
	}

	return 0;
}

int gxtkAudio::StopChannel( int channel )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->Stop();
	}

	return 0;
}

int gxtkAudio::PauseChannel( int channel )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->Pause();
	}

	return 0;
}

int gxtkAudio::ResumeChannel( int channel )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->Play();
		SetChannelVolume(channel);
	}
	
	return 0;
}

int gxtkAudio::ChannelState( int channel )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);
	
  if (soundInstance != NULL)
	{
		if (soundInstance->IsPlaying() == true)
		{
			return 1;
		}
		else
		{
			if (soundInstance->IsPaused() == true)
			{
				return 2;
			}
		}
	}

	return 0;
}

int gxtkAudio::SetVolume( int channel,float volume )
{
	channelVolume[channel] = volume;

	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->SetVolume(volume);
	}

	return 0;
}

int gxtkAudio::SetChannelVolume(int channel)
{
	return SetVolume(channel, channelVolume[channel]);
}

int gxtkAudio::SetPan( int channel,float pan )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->SetPan((int)(pan * 10000));
	}

	return 0;
}

int gxtkAudio::SetRate( int channel,float rate )
{
	SoundInstance *soundInstance = gPopCapApp->mSoundManager->GetChannel(channel);

	if (soundInstance != NULL)
	{
		soundInstance->AdjustPitch(rate);
	}
	
	return 0;
}

int gxtkAudio::PlayMusic( String path,int flags )
{
	StopMusic();
	
	gxtkSample *music=LoadSample( path );
	if( !music ) return -1;
	
	PlaySample( music,32,flags );
	return 0;
}

int gxtkAudio::StopMusic()
{
	StopChannel( 32 );
	return 0;
}

int gxtkAudio::PauseMusic()
{
	PauseChannel( 32 );
	return 0;
}

int gxtkAudio::ResumeMusic()
{
	ResumeChannel( 32 );
	
	return 0;
}

int gxtkAudio::MusicState()
{
	return ChannelState( 32 );
}

int gxtkAudio::SetMusicVolume( float volume )
{
	SetVolume( 32,volume );
	return 0;
}

gxtkSample::gxtkSample()
{
	id = 0;
}

gxtkSample::gxtkSample( int buf )
{
}

gxtkSample::~gxtkSample()
{
	Discard();
}

void gxtkSample::SetBuffer( int buf )
{
}

int gxtkSample::Discard()
{
	gPopCapApp->mSoundManager->ReleaseSound(id);

	return 0;
}

//functions
static void SetGraphicsNative(int width, int height)
{
	gPopCapApp->mWidth = width;
	gPopCapApp->mHeight = height;
	
	gPopCapApp->SwitchScreenMode(gPopCapApp->mIsWindowed);
	gPopCapApp->mWidgetManager->ResizeWidget(Rect(0, 0, width, height), Rect(0, 0, width, height));
}

//functions
static void SwitchScreenNative(bool fullScreen)
{
	gPopCapApp->SwitchScreenMode(!fullScreen);
}

static bool IsFullscreenNative()
{
	return !gPopCapApp->mIsWindowed;
}

static void SetLinearBlendNative(bool blend)
{
	gPopCapGraphics->SetLinearBlend(blend);
}

bool gPopCapCenterWindow = false;
static void CenterWindowNative(bool center)
{
	gPopCapCenterWindow = center;
}

DDImage* gPopCapDDImage = NULL;
static void BeginRenderSexyNative()
{
	gPopCapGraphics = new Graphics(gPopCapApp->mWidgetManager->mImage);
}

static void EndRenderSexyNative()
{
	delete gPopCapGraphics;
	gPopCapGraphics = NULL;

	gPopCapApp->Redraw(NULL);
}
