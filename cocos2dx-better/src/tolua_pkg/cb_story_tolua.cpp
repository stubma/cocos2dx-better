/*
** Lua binding: story
** Generated automatically by tolua++-1.0.92 on Fri Jun 27 15:55:45 2014.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_story_open (lua_State* tolua_S);

#include "CCStoryCommandSet.h" 

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: msgsize */
#ifndef TOLUA_DISABLE_tolua_story_msgsize00
static int tolua_story_msgsize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float size = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   msgsize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msgsize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: msgcolor */
#ifndef TOLUA_DISABLE_tolua_story_msgcolor00
static int tolua_story_msgcolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int c = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   msgcolor(c);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msgcolor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: msgpos */
#ifndef TOLUA_DISABLE_tolua_story_msgpos00
static int tolua_story_msgpos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   msgpos(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msgpos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: msganchor */
#ifndef TOLUA_DISABLE_tolua_story_msganchor00
static int tolua_story_msganchor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   msganchor(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msganchor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: namepos */
#ifndef TOLUA_DISABLE_tolua_story_namepos00
static int tolua_story_namepos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   namepos(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'namepos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: namecolor */
#ifndef TOLUA_DISABLE_tolua_story_namecolor00
static int tolua_story_namecolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int c = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   namecolor(c);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'namecolor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: namesize */
#ifndef TOLUA_DISABLE_tolua_story_namesize00
static int tolua_story_namesize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float size = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   namesize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'namesize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: nameanchor */
#ifndef TOLUA_DISABLE_tolua_story_nameanchor00
static int tolua_story_nameanchor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   nameanchor(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'nameanchor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: labelcolor */
#ifndef TOLUA_DISABLE_tolua_story_labelcolor00
static int tolua_story_labelcolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int c = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   labelcolor(c);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'labelcolor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: labelsize */
#ifndef TOLUA_DISABLE_tolua_story_labelsize00
static int tolua_story_labelsize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float size = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   labelsize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'labelsize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: labelanchor */
#ifndef TOLUA_DISABLE_tolua_story_labelanchor00
static int tolua_story_labelanchor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   labelanchor(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'labelanchor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: msg */
#ifndef TOLUA_DISABLE_tolua_story_msg00
static int tolua_story_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  string s = ((string)  tolua_tocppstring(tolua_S,2,0));
  {
   msg(name,s);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: move */
#ifndef TOLUA_DISABLE_tolua_story_move00
static int tolua_story_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float time = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   move(name,x,y,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: wait */
#ifndef TOLUA_DISABLE_tolua_story_wait00
static int tolua_story_wait00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float time = ((float)  tolua_tonumber(tolua_S,1,0));
  {
   wait(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wait'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: waitarm */
#ifndef TOLUA_DISABLE_tolua_story_waitarm00
static int tolua_story_waitarm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   waitarm();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'waitarm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: waitclick */
#ifndef TOLUA_DISABLE_tolua_story_waitclick00
static int tolua_story_waitclick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   waitclick();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'waitclick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: fadein */
#ifndef TOLUA_DISABLE_tolua_story_fadein00
static int tolua_story_fadein00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float time = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   fadein(name,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fadein'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: fadeout */
#ifndef TOLUA_DISABLE_tolua_story_fadeout00
static int tolua_story_fadeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float time = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   fadeout(name,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fadeout'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: flash */
#ifndef TOLUA_DISABLE_tolua_story_flash00
static int tolua_story_flash00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  unsigned int color = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  float times = ((float)  tolua_tonumber(tolua_S,3,0));
  float time = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   flash(name,color,times,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flash'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: shake */
#ifndef TOLUA_DISABLE_tolua_story_shake00
static int tolua_story_shake00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float amplitude = ((float)  tolua_tonumber(tolua_S,2,0));
  float time = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   shake(name,amplitude,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shake'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: scale */
#ifndef TOLUA_DISABLE_tolua_story_scale00
static int tolua_story_scale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float to = ((float)  tolua_tonumber(tolua_S,2,0));
  float time = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   scale(name,to,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'scale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: img */
#ifndef TOLUA_DISABLE_tolua_story_img00
static int tolua_story_img00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  string frameName = ((string)  tolua_tocppstring(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   img(name,frameName,x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'img'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: label */
#ifndef TOLUA_DISABLE_tolua_story_label00
static int tolua_story_label00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   label(name,text,x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'label'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: arm */
#ifndef TOLUA_DISABLE_tolua_story_arm00
static int tolua_story_arm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  string armName = ((string)  tolua_tocppstring(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   arm(name,armName,x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'arm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: armplay */
#ifndef TOLUA_DISABLE_tolua_story_armplay00
static int tolua_story_armplay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  string animName = ((string)  tolua_tocppstring(tolua_S,2,0));
  {
   armplay(name,animName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'armplay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: angle */
#ifndef TOLUA_DISABLE_tolua_story_angle00
static int tolua_story_angle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float degree = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   angle(name,degree);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'angle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: rotate */
#ifndef TOLUA_DISABLE_tolua_story_rotate00
static int tolua_story_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  float delta = ((float)  tolua_tonumber(tolua_S,2,0));
  float time = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   rotate(name,delta,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: bgpos */
#ifndef TOLUA_DISABLE_tolua_story_bgpos00
static int tolua_story_bgpos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,1,0));
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   bgpos(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bgpos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: bg */
#ifndef TOLUA_DISABLE_tolua_story_bg00
static int tolua_story_bg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string frameName = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   bg(frameName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: bgcolor */
#ifndef TOLUA_DISABLE_tolua_story_bgcolor00
static int tolua_story_bgcolor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int c = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   bgcolor(c);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bgcolor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: z */
#ifndef TOLUA_DISABLE_tolua_story_z00
static int tolua_story_z00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  int zOrder = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   z(name,zOrder);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'z'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: bgm */
#ifndef TOLUA_DISABLE_tolua_story_bgm00
static int tolua_story_bgm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string musicFile = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   bgm(musicFile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bgm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: sound */
#ifndef TOLUA_DISABLE_tolua_story_sound00
static int tolua_story_sound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string soundFile = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   sound(soundFile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: remove */
#ifndef TOLUA_DISABLE_tolua_story_remove00
static int tolua_story_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   remove(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: show */
#ifndef TOLUA_DISABLE_tolua_story_show00
static int tolua_story_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   show(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: hide */
#ifndef TOLUA_DISABLE_tolua_story_hide00
static int tolua_story_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   hide(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: flipx */
#ifndef TOLUA_DISABLE_tolua_story_flipx00
static int tolua_story_flipx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   flipx(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flipx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: flipy */
#ifndef TOLUA_DISABLE_tolua_story_flipy00
static int tolua_story_flipy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string name = ((string)  tolua_tocppstring(tolua_S,1,0));
  {
   flipy(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flipy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_story_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"msgsize",tolua_story_msgsize00);
  tolua_function(tolua_S,"msgcolor",tolua_story_msgcolor00);
  tolua_function(tolua_S,"msgpos",tolua_story_msgpos00);
  tolua_function(tolua_S,"msganchor",tolua_story_msganchor00);
  tolua_function(tolua_S,"namepos",tolua_story_namepos00);
  tolua_function(tolua_S,"namecolor",tolua_story_namecolor00);
  tolua_function(tolua_S,"namesize",tolua_story_namesize00);
  tolua_function(tolua_S,"nameanchor",tolua_story_nameanchor00);
  tolua_function(tolua_S,"labelcolor",tolua_story_labelcolor00);
  tolua_function(tolua_S,"labelsize",tolua_story_labelsize00);
  tolua_function(tolua_S,"labelanchor",tolua_story_labelanchor00);
  tolua_function(tolua_S,"msg",tolua_story_msg00);
  tolua_function(tolua_S,"move",tolua_story_move00);
  tolua_function(tolua_S,"wait",tolua_story_wait00);
  tolua_function(tolua_S,"waitarm",tolua_story_waitarm00);
  tolua_function(tolua_S,"waitclick",tolua_story_waitclick00);
  tolua_function(tolua_S,"fadein",tolua_story_fadein00);
  tolua_function(tolua_S,"fadeout",tolua_story_fadeout00);
  tolua_function(tolua_S,"flash",tolua_story_flash00);
  tolua_function(tolua_S,"shake",tolua_story_shake00);
  tolua_function(tolua_S,"scale",tolua_story_scale00);
  tolua_function(tolua_S,"img",tolua_story_img00);
  tolua_function(tolua_S,"label",tolua_story_label00);
  tolua_function(tolua_S,"arm",tolua_story_arm00);
  tolua_function(tolua_S,"armplay",tolua_story_armplay00);
  tolua_function(tolua_S,"angle",tolua_story_angle00);
  tolua_function(tolua_S,"rotate",tolua_story_rotate00);
  tolua_function(tolua_S,"bgpos",tolua_story_bgpos00);
  tolua_function(tolua_S,"bg",tolua_story_bg00);
  tolua_function(tolua_S,"bgcolor",tolua_story_bgcolor00);
  tolua_function(tolua_S,"z",tolua_story_z00);
  tolua_function(tolua_S,"bgm",tolua_story_bgm00);
  tolua_function(tolua_S,"sound",tolua_story_sound00);
  tolua_function(tolua_S,"remove",tolua_story_remove00);
  tolua_function(tolua_S,"show",tolua_story_show00);
  tolua_function(tolua_S,"hide",tolua_story_hide00);
  tolua_function(tolua_S,"flipx",tolua_story_flipx00);
  tolua_function(tolua_S,"flipy",tolua_story_flipy00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_story (lua_State* tolua_S) {
 return tolua_story_open(tolua_S);
};
#endif

