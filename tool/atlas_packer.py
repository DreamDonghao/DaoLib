import json
import os
import sys
from PIL import Image

# ========== 配置 ==========
ROOT_DIR = "example/input_images/images"
OUTPUT_JSON = "example/atlas_meta.json"
OUTPUT_BASE = "example/atlas/atlas"
PADDING = 2
ATLAS_WIDTH = 2048
ATLAS_HEIGHT = 2048

print("正在扫描 PNG...\n")

def make_json_atlas_path(path: str) -> str:
    return os.path.basename(path)

# ========== 扫描 PNG ==========
images_to_pack = []
sequence_groups = {}   # 新增：记录序列帧 {folder_name: [items...]}

for root, _, files in os.walk(ROOT_DIR):
    folder_name = os.path.basename(root)

    for f in files:
        if not f.lower().endswith(".png"):
            continue

        full = os.path.join(root, f)

        if f.startswith(f"{OUTPUT_BASE}_"):
            continue

        try:
            with Image.open(full) as img:
                img.load()
                mode, size, data = img.mode, img.size, img.tobytes()
            img = Image.frombytes(mode, size, data)

            rel = os.path.relpath(full, ROOT_DIR).replace("\\", "/")
            key = os.path.splitext(rel)[0].replace("/", "_")

            item = {
                "rel": rel,
                "key": key,
                "img": img,
                "w": img.width,
                "h": img.height,
                "folder": folder_name
            }

            # ========= 关键：识别 sf 文件夹 =========
            if folder_name.startswith("sf"):
                if folder_name not in sequence_groups:
                    sequence_groups[folder_name] = []
                sequence_groups[folder_name].append(item)
                print("序列帧:", rel)
            else:
                images_to_pack.append(item)
                print("普通图片:", rel)

        except Exception as e:
            print("跳过错误图片:", full, e)

if not images_to_pack and not sequence_groups:
    print("没有 PNG 文件")
    sys.exit(1)

os.makedirs(os.path.dirname(OUTPUT_BASE), exist_ok=True)

# 所有图片统一参与图集打包（包括序列帧）
all_items = images_to_pack[:]
for v in sequence_groups.values():
    all_items.extend(v)

all_items.sort(key=lambda x_: x_["h"], reverse=True)

# ========== 图集打包 ==========
atlas_index = 2
cur = 0

image_metadata = {}
sequence_metadata = {}

# 先创建 sequence 容器
for seq_name in sequence_groups.keys():
    sequence_metadata[seq_name] = []

while cur < len(all_items):
    atlas_name = f"{OUTPUT_BASE}_{atlas_index:04d}.png"
    atlas = Image.new("RGBA", (ATLAS_WIDTH, ATLAS_HEIGHT))
    print(f"\n--- 打包: {atlas_name} ---")

    x = y = row_h = 0
    i = cur
    packed_count = 0

    while i < len(all_items):
        item = all_items[i]
        w, h = item["w"], item["h"]

        new_line = (x + w > ATLAS_WIDTH)
        ny = y + row_h + PADDING if new_line else y

        if ny + h > ATLAS_HEIGHT:
            print(f"放不下，换下个图集: {item['rel']}")
            break

        if new_line:
            x = 0
            y = ny
            row_h = 0

        if x + w > ATLAS_WIDTH:
            print(f"警告：图片 {item['rel']} 宽度 {w} 超过图集宽度，跳过。")
            i += 1
            continue

        atlas.paste(item["img"], (x, y))
        packed_count += 1

        frame_meta = {
            "atlas_file": make_json_atlas_path(atlas_name),
            "atlas_id": atlas_index,
            "left": x,
            "top": y,
            "right": x + w,
            "bottom": y + h
        }

        # ========= 分流写入 =========
        if item["folder"].startswith("sf"):
            sequence_metadata[item["folder"]].append(frame_meta)
        else:
            image_metadata[item["key"]] = frame_meta

        x += w + PADDING
        row_h = max(row_h, h)
        i += 1

    if packed_count > 0:
        atlas.save(atlas_name)
        print(f"完成: {atlas_name} (包含 {packed_count} 张图片)")
    else:
        break

    atlas_index += 1
    cur = i

# ========== 输出 JSON ==========
final_json = {
    "images": image_metadata,
    "sequence_frames": sequence_metadata
}

with open(OUTPUT_JSON, "w", encoding="utf8") as f:
    json.dump(final_json, f, indent=2, ensure_ascii=False)

print("\n===============================")
print("图集生成完毕")
print("输出 JSON:", OUTPUT_JSON)
print(f"输出图集文件: {OUTPUT_BASE}_XXXX.png")
