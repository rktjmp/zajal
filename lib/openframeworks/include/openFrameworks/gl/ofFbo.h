#pragma once

#include "ofTexture.h"

//#ifndef TARGET_OPENGLES



class ofFbo {
public:
	struct Settings;
	
	ofFbo();
	ofFbo(const ofFbo & mom);
	ofFbo & operator=(const ofFbo & fbo);
	virtual ~ofFbo();

	void allocate(int width, int height, int internalformat = GL_RGBA, int numSamples = 0);
	void allocateForShadow( int width, int height );
	void allocate(Settings settings = Settings());
	
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);
	
	int getWidth();
	int getHeight();
	
	void begin();
	void end();
	
	ofTexture& getTexture(int attachmentPoint = 0);		// get texture at attachment position
	
	void readToPixels(ofPixels & pixels, int attachmentPoint = 0);
	void readToPixels(ofShortPixels & pixels, int attachmentPoint = 0);
	void readToPixels(ofFloatPixels & pixels, int attachmentPoint = 0);

	// advanced functions
	void bind();
	void unbind();
	
	int	getNumTextures();

	GLuint getFbo();	// returns GLuint of Fbo for advanced actions
	
	
	static void	checkGLSupport();
	static int maxColorAttachments();	// return max color attachments
	static int maxDrawBuffers();		// return max simultaneous draw buffers
	static int maxSamples();			// return max MSAA samples

	GLuint getDepthBuffer(){ return depthBuffer; }
	GLuint getStencilBuffer(){ return stencilBuffer; }

	struct Settings {
		int		width;					// width of images attached to fbo
		int		height;					// height of images attached to fbo
		int		numColorbuffers;		// how many color buffers to create
		bool	useDepth;				// whether to use depth buffer or not
		bool	useStencil;				// whether to use stencil buffer or not
		GLenum	textureTarget;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
		GLint	internalformat;			// GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
		int		wrapModeHorizontal;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
		int		wrapModeVertical;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
		int		minFilter;				// GL_NEAREST, GL_LINEAR etc.
		int		maxFilter;				// GL_NEAREST, GL_LINEAR etc.
		int		numSamples;				// number of samples for multisampling (set 0 to disable)
		
		Settings();
	};
private:

	Settings 			settings;
	int					isBound;

	GLuint				fbo;			// main fbo which we bind for drawing into, all renderbuffers are attached to this
	GLuint				fboTextures;	// textures are attached to this (if MSAA is disabled, this is equal to fbo, otherwise it's a new fbo)
	GLuint				depthBuffer;
	GLuint				stencilBuffer;

	GLint				savedFramebuffer;	// save bound framebuffer before switching

	vector<GLuint>		colorBuffers;	// only used if using MSAA
	vector<ofTexture>	textures;

	static int			_maxColorAttachments;
	static int			_maxDrawBuffers;
	static int			_maxSamples;
	
	bool				dirty;

	void destroy();
	
	bool checkStatus();
	void createAndAttachTexture(GLenum attachmentPoint);
	GLuint createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint);

	// if using MSAA, we will have rendered into a colorbuffer, not directly into the texture
	// call this to blit from the colorbuffer into the texture so we can use the results for rendering, or input to a shader etc.
	void updateTexture(int attachmentPoint);

};

//#endif
