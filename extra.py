env = DefaultEnvironment()

env.Append(CPPPATH=["$PROJECT_DIR/lib/cube/Inc"])

flags = [
   "-mfloat-abi=hard",
   "-mfpu=fpv4-sp-d16"
]
env.Append(CCFLAGS=flags, LINKFLAGS=flags)
