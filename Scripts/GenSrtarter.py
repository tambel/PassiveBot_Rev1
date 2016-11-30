ROOT="D:"
DEV_DIR="Bot"
DEPS_DIR="Dependencies"
PROJ_DIR="Projects"

DEV_BOT_PATH="{}\{}".format(ROOT, DEV_DIR)
DEPS_PATH="{}\{}".format(DEV_BOT_PATH,DEPS_DIR)

PROJ_PATH="{}\{}".format(DEV_BOT_PATH, PROJ_DIR)
VS_DEVENV_DIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE"

ADD_DEPS_VAR="setx BOT_DEV_DEPS {}\{}\{}".format(ROOT,DEV_DIR,DEPS_DIR)

vars={"BOOST":"boost\\boost_1_60_0",
      "MYSQL":"mysql-connector-c++-noinstall-1.1.7-win32",
      "AUTOIT":"AutoIt",
      "DIRECTX":"Microsoft DirectX SDK (June 2010)",
      "RECASTDETOUR": "RecastDetour",
      "OGRE":"Ogre"}
add_vars=""

for k,v in vars.items():
    add_vars+="setx {name} \"{deps}\{path}\"\n".format(name=k,path=v,deps=DEPS_PATH)

start_vs="\"{}\devenv.com\" \"{}\PassiveBot_Rev1\PassiveBot_0.1.sln\"\n".format(VS_DEVENV_DIR, PROJ_PATH)

result=add_vars

rf=open("start.bat", "w")

rf.write(result)

print(result)