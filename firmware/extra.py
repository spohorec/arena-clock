import os

env = DefaultEnvironment()

def generate_stm32_cube_sources():
    lib_path="lib/cube"
    print(lib_path+"/arena-clock.ioc")
    if not os.path.exists(lib_path+"/arena-clock.ioc"):
        # fail the build
        print("Can't find ioc file")
        return
    if os.path.exists(lib_path+"/Src/"):
        # check time
        ioc_time = os.path.getmtime(lib_path+"/arena-clock.ioc")
        src_time = os.path.getmtime(lib_path+"/Src/")
        if (ioc_time < src_time):
            print("Cube sources up to date")
            return

    # rebuild
    print("Rebuilding cube sources...")
    env.Execute(lib_path+"/generate_cube_code.sh")

generate_stm32_cube_sources()

env.Append(CPPPATH=["$PROJECT_DIR/lib/cube/Inc"])

flags = [
   "-mfloat-abi=hard",
   "-mfpu=fpv4-sp-d16"
]
env.Append(CCFLAGS=flags, LINKFLAGS=flags)
