import json
import re

with open("example/atlas_meta.json", "r", encoding="utf-8") as f:
    data = json.load(f)

images = data.get("images", {})
sequences = data.get("sequence_frames", {})

# 函数：将文件名或路径转为合法 C++ 常量名
def to_enum_name(s: str) -> str:
    s = s.lower()
    s = re.sub(r"[./\- ]", "_", s)
    return s

# ===============================
# textures.inc
# ===============================
with open("example/inc/textures.inc", "w", encoding="utf-8") as f:
    texture_id = 2
    f.write(f"#pragma once\n")
    f.write("using TextureID = uint32_t;\n")
    # 普通图片
    for key in images.keys():
        cpp_name = to_enum_name(key)
        f.write(f"constexpr TextureID {cpp_name} = {texture_id};\n")
        texture_id += 1

    # 序列帧：只生成 begin / end
    for seq_name, frames in sequences.items():
        cpp_name = to_enum_name(seq_name)
        begin_id = texture_id
        end_id = texture_id + len(frames)

        f.write(f"constexpr TextureID {cpp_name}_begin = {begin_id};\n")
        f.write(f"constexpr TextureID {cpp_name}_end = {end_id};\n")

        texture_id += len(frames)

# ===============================
# texture_datas.inc
# ===============================
region_entries = []

# 普通图片
for name, v in images.items():
    region_entries.append(
        f'{{"{v["atlas_file"]}",{v["atlas_id"]},{v["left"]/2048},{v["top"]/2048},{v["right"]/2048},{v["bottom"]/2048}}}'
    )

# 序列帧（顺序很重要，和 begin/end 对应）
for seq_name, frames in sequences.items():
    for v in frames:
        region_entries.append(
            f'{{"{v["atlas_file"]}",{v["atlas_id"]},{v["left"]/2048},{v["top"]/2048},{v["right"]/2048},{v["bottom"]/2048}}}'
        )

with open("example/inc/texture_datas.inc", "w", encoding="utf-8") as f:
    f.write(",".join(region_entries))

print("C++ 风格 inc 生成完成")
