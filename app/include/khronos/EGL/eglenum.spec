# This is the EGL enumerant registry.
#
# It is an extremely important file. Only the Khronos API Registrar
# (currently Jon Leech) should change it.
#
# Rules for enum allocation are the same as for the OpenGL enumerant
# registry. To obtain an EGL enumerant allocation, submit a request in
# the Khronos Bugzilla for Product "EGL", component "Registry".

# EGL Versioning
Extensions define:
	VERSION_1_0					= 1
	VERSION_1_1					= 1
	VERSION_1_2					= 1
	VERSION_1_3					= 1

# EGL Enumerants.

EGLBoolean enum:
	FALSE						= 0
	TRUE						= 1

# These values may vary depending on semantics of native concepts
# DONT_CARE is an attribute value
EGLOutOfBand enum:
	DEFAULT_DISPLAY					= ((void *)0)
	NO_CONTEXT					= ((EGLContext)0)
	NO_DISPLAY					= ((EGLDisplay)0)
	NO_SURFACE					= ((EGLSurface)0)
	DONT_CARE					= ((EGLint)-1)

# Errors / GetError return values
EGLError enum:
	SUCCESS						= 0x3000
	NOT_INITIALIZED					= 0x3001
	BAD_ACCESS					= 0x3002
	BAD_ALLOC					= 0x3003
	BAD_ATTRIBUTE					= 0x3004
	BAD_CONFIG					= 0x3005
	BAD_CONTEXT					= 0x3006
	BAD_CURRENT_SURFACE				= 0x3007
	BAD_DISPLAY					= 0x3008
	BAD_MATCH					= 0x3009
	BAD_NATIVE_PIXMAP				= 0x300A
	BAD_NATIVE_WINDOW				= 0x300B
	BAD_PARAMETER					= 0x300C
	BAD_SURFACE					= 0x300D
	CONTEXT_LOST					= 0x300E
# Reserved for Graham Connor, Imagination Tech.
	CONTEXT_LOST_IMG				= 0x300E
# Khronos_future_use: 0x300F-0x301F (for additional errors)

# Config attribute names
EGLConfigAttrib enum:
	BUFFER_SIZE					= 0x3020
	ALPHA_SIZE					= 0x3021
	BLUE_SIZE					= 0x3022
	GREEN_SIZE					= 0x3023
	RED_SIZE					= 0x3024
	DEPTH_SIZE					= 0x3025
	STENCIL_SIZE					= 0x3026
	CONFIG_CAVEAT					= 0x3027
	CONFIG_ID					= 0x3028
	LEVEL						= 0x3029
	MAX_PBUFFER_HEIGHT				= 0x302A
	MAX_PBUFFER_PIXELS				= 0x302B
	MAX_PBUFFER_WIDTH				= 0x302C
	NATIVE_RENDERABLE				= 0x302D
	NATIVE_VISUAL_ID				= 0x302E
	NATIVE_VISUAL_TYPE				= 0x302F
	PRESERVED_RESOURCES				= 0x3030
	SAMPLES						= 0x3031
	SAMPLE_BUFFERS					= 0x3032
	SURFACE_TYPE					= 0x3033
	TRANSPARENT_TYPE				= 0x3034
	TRANSPARENT_BLUE_VALUE				= 0x3035
	TRANSPARENT_GREEN_VALUE				= 0x3036
	TRANSPARENT_RED_VALUE				= 0x3037
	NONE						= 0x3038 # Attrib list terminator
	BIND_TO_TEXTURE_RGB				= 0x3039
	BIND_TO_TEXTURE_RGBA				= 0x303A
	MIN_SWAP_INTERVAL				= 0x303B
	MAX_SWAP_INTERVAL				= 0x303C
	LUMINANCE_SIZE					= 0x303D
	ALPHA_MASK_SIZE					= 0x303E
	COLOR_BUFFER_TYPE				= 0x303F
	RENDERABLE_TYPE					= 0x3040
	MATCH_NATIVE_PIXMAP				= 0x3041
	CONFORMANT					= 0x3042
# EGL_KHR_config_attribs
	CONFORMANT_KHR					= 0x3042
# EGL_KHR_lock_surface
	MATCH_FORMAT_KHR				= 0x3043

# Khronos_future_use: 0x3044-0x304F (for additional config attributes)

# Config attribute values
EGLConfigAttribValue enum:
	SLOW_CONFIG					= 0x3050 # CONFIG_CAVEAT value
	NON_CONFORMANT_CONFIG				= 0x3051 # CONFIG_CAVEAT value
	TRANSPARENT_RGB					= 0x3052 # TRANSPARENT_TYPE value
	NO_TEXTURE					= 0x305C # TEXTURE_FORMAT/TARGET value
	TEXTURE_RGB					= 0x305D # TEXTURE_FORMAT value
	TEXTURE_RGBA					= 0x305E # "
	TEXTURE_2D					= 0x305F # TEXTURE_TARGET value
	RGB_BUFFER					= 0x308E # COLOR_BUFFER_TYPE value
	LUMINANCE_BUFFER				= 0x308F # "

# Config attribute mask bits
EGLSurfaceTypeMask enum:
	PBUFFER_BIT					= 0x0001 # SURFACE_TYPE mask bit
	PIXMAP_BIT					= 0x0002 # "
	WINDOW_BIT					= 0x0004 # "
# EGL_TAO_image_surface (approximate - spec not registered yet - owned by Phil Huxley)
	PBUFFER_IMAGE_BIT_TAO				= 0x0008 # "
	PBUFFER_PALETTE_IMAGE_BIT_TAO			= 0x0010 # "
# EGL 1.3 bits
	VG_COLORSPACE_LINEAR_BIT			= 0x0020 # "
	VG_ALPHA_FORMAT_PRE_BIT				= 0x0040 # "
# EGL_KHR_config_attribs bits
	VG_COLORSPACE_LINEAR_BIT_KHR			= 0x0020 # "
	VG_ALPHA_FORMAT_PRE_BIT_KHR			= 0x0040 # "
# EGL_KHR_lock_surface bits
	LOCK_SURFACE_BIT_KHR				= 0x0080 # "
	OPTIMAL_FORMAT_BIT_KHR				= 0x0100 # "
# EGL 1.4 bits
	MULTISAMPLE_RESOLVE_BOX_BIT			= 0x0200 # "
	SWAP_BEHAVIOR_PRESERVED_BIT			= 0x0400 # "

EGLRenderableTypeMask enum:
	OPENGL_ES_BIT					= 0x0001 # RENDERABLE_TYPE mask bit
	OPENVG_BIT					= 0x0002 # "
	OPENGL_ES2_BIT					= 0x0004 # "
	OPENGL_BIT					= 0x0008 # "    # EGL 1.4

# QueryString targets
EGLQueryString enum:
	VENDOR						= 0x3053
	VERSION						= 0x3054
	EXTENSIONS					= 0x3055
	CLIENT_APIS					= 0x308D

# QuerySurface / SurfaceAttrib / CreatePbufferSurface targets
EGLSurfaceAttrib enum:
	HEIGHT						= 0x3056
	WIDTH						= 0x3057
	LARGEST_PBUFFER					= 0x3058
	TEXTURE_FORMAT					= 0x3080 # For pbuffers bound as textures
	TEXTURE_TARGET					= 0x3081 # "
	MIPMAP_TEXTURE					= 0x3082 # "
	MIPMAP_LEVEL					= 0x3083 # "
	RENDER_BUFFER					= 0x3086
	VG_COLORSPACE					= 0x3087
	COLORSPACE					= VG_COLORSPACE		# EGL 1.3 token renaming
	VG_ALPHA_FORMAT					= 0x3088
	ALPHA_FORMAT					= VG_ALPHA_FORMAT	# EGL 1.3 token renaming
	HORIZONTAL_RESOLUTION				= 0x3090
	VERTICAL_RESOLUTION				= 0x3091
	PIXEL_ASPECT_RATIO				= 0x3092
	SWAP_BEHAVIOR					= 0x3093
	MULTISAMPLE_RESOLVE				= 0x3099 # EGL 1.4 SurfaceAttrib

# COLORSPACE surface attribute values
EGLColorspace enum:
	VG_COLORSPACE_sRGB				= 0x3089
	VG_COLORSPACE_LINEAR				= 0x308A
	COLORSPACE_sRGB					= VG_COLORSPACE_sRGB	# EGL 1.3 token renaming
	COLORSPACE_LINEAR				= VG_COLORSPACE_LINEAR	# EGL 1.3 token renaming

# ALPHA_FORMAT surface attribute values
EGLAlphaFormat enum:
	VG_ALPHA_FORMAT_NONPRE				= 0x308B
	VG_ALPHA_FORMAT_PRE				= 0x308C
	ALPHA_FORMAT_NONPRE				= VG_ALPHA_FORMAT_NONPRE # EGL 1.3 token renaming
	ALPHA_FORMAT_PRE				= VG_ALPHA_FORMAT_PRE	 # EGL 1.3 token renaming

# SWAP_BEHAVIOR surface attribute values
EGLSwapBehavior enum:
	BUFFER_PRESERVED				= 0x3094
	BUFFER_DESTROYED				= 0x3095

# MULTISAMPLE_RESOLVE surface attribute values
EGLMultisampleResolve enum:
	MULTISAMPLE_RESOLVE_DEFAULT			= 0x309A # EGL 1.4
	MULTISAMPLE_RESOLVE_BOX				= 0x309B # EGL 1.4

# Constant values related to HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION,
#   and PIXEL_ASPECT_RATIO.
# DISPLAY_SCALING is a scale factor used to represent fractional values
#   as integers.
# UNKNOWN is returned for resolution & aspect ratio of offscreen
#   surfaces.
EGLDisplayScaling enum:
	DISPLAY_SCALING					= 10000
	UNKNOWN						= ((EGLint)-1)

# CreatePbufferFromClientBuffer buffer type
EGLClientBufferType enum:
	OPENVG_IMAGE					= 0x3096

# QueryContext target
EGLQueryContextTarget enum:
	CONTEXT_CLIENT_TYPE				= 0x3097

# CreateContext attributes
EGLCreateContextAttrib enum:
	CONTEXT_CLIENT_VERSION				= 0x3098

# Khronos_future_use: 0x309C-0x309F

# BindAPI <api> target
EGLBindAPITarget enum:
	OPENGL_ES_API					= 0x30A0
	OPENVG_API					= 0x30A1
	OPENGL_API					= 0x30A2    # EGL 1.4

# Khronos_future_use: 0x30A3-0x30AF (reserved for additional client API names)

# BindTexImage / ReleaseTexImage buffer target, and
# RENDER_BUFFER surface attribute values
EGLBindBufferTarget enum:
	BACK_BUFFER					= 0x3084
	SINGLE_BUFFER					= 0x3085

# GetCurrentSurface targets
EGLSurfaceName enum:
	DRAW						= 0x3059
	READ						= 0x305A

# WaitNative engines
EGLEngine enum:
	CORE_NATIVE_ENGINE				= 0x305B

###############################################################################

# Khronos: 0x3000-0x305F (used, or marked as reserved above)

###############################################################################

# Tao: 0x3060-0x306F
# Reserved for Phil Huxley

# TAO_future_use: 0x3060-0x306F

###############################################################################

# Nokia: 0x3070-0x307F
# Reserved for Jani Vaarala

# NOK_future_use: 0x3070-0x307F (Jani Vaarala, Nokia)

###############################################################################

# Khronos: 0x3080-0x30AF (used, or marked as reserved above)

###############################################################################

# NVIDIA: 0x30B0-0x30BF
# Reserved for Ignacio Llamas

# EGL Image extension names corresponding to these enums TBD once final specs are ready
#	EGL_NATIVE_PIXMAP_KHR			  0x30B0    # eglCreateImageKHR targets
#	EGL_GL_TEXTURE_2D_KHR			  0x30B1
#	EGL_GL_TEXTURE_3D_KHR			  0x30B2
#	EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR	  0x30B3
#	EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR	  0x30B4
#	EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR	  0x30B5
#	EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR	  0x30B6
#	EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR	  0x30B7
#	EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR	  0x30B8
#	EGL_GL_RENDERBUFFER_KHR			  0x30B9
#	EGL_VG_PARENT_IMAGE_KHR			  0x30BA
#	EGL_GL_TEXTURE_RECTANGLE_NV_KHR		  0x30BB
#	EGL_GL_TEXTURE_LEVEL_KHR		  0x30BC    # eglCreateImageKHR attributes
#	EGL_GL_TEXTURE_ZOFFSET_KHR		  0x30BD

# NV_future_use: 0x30BE-0x30BF

###############################################################################

# Khronos: 0x30C0-0x30CF

# EGL_KHR_lock_surface
	EGL_FORMAT_RGB_565_EXACT_KHR			= 0x30C0
	EGL_FORMAT_RGB_565_KHR				= 0x30C1
	EGL_FORMAT_RGBA_8888_EXACT_KHR			= 0x30C2
	EGL_FORMAT_RGBA_8888_KHR			= 0x30C3
	EGL_MAP_PRESERVE_PIXELS_KHR			= 0x30C4
	EGL_LOCK_USAGE_HINT_KHR				= 0x30C5
	EGL_BITMAP_POINTER_KHR				= 0x30C6
	EGL_BITMAP_PITCH_KHR				= 0x30C7
	EGL_BITMAP_ORIGIN_KHR				= 0x30C8
	EGL_BITMAP_PIXEL_RED_OFFSET_KHR			= 0x30C9
	EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR		= 0x30CA
	EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR		= 0x30CB
	EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR		= 0x30CC
	EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR		= 0x30CD
	EGL_LOWER_LEFT_KHR				= 0x30CE
	EGL_UPPER_LEFT_KHR				= 0x30CF

###############################################################################

# Symbian: 0x30D0-0x30DF
# Reserved for Robert Palmer (bug #2545)

# SYM_future_use: 0x30D0-0x30DF

###############################################################################

# NVIDIA: 0x30E0-0x30EF
# Reserved for Russell Pflughaupt (bug #3314)

# NV_future_use: 0x30E0-0x30EF

###############################################################################
### Please remember that new EGL enum allocations must be obtained by request
### to the Khronos API Registrar (see comments at the top of this file).
### File requests in the Khronos Bugzilla, Product "EGL", Component "Registry"
###############################################################################

# Any_vendor_future_use: 0x30F0-0x3FFF
#
#   This range must be the last range in the file.  To generate a new
#   range, allocate multiples of 16 from the beginning of the
#   Any_vendor_future_use range and update eglenum.spec.
