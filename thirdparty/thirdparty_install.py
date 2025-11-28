# 自动下载并安装环境配置

import os
import zipfile
import tqdm


current_dir = os.path.dirname(os.path.abspath(__file__))
print(current_dir)

SDL3_DIR = os.path.join(current_dir, "SDL3")
SDL3_INSTALL_DIR = r"C:\ENV\SDL3"

GLM_DIR = os.path.join(current_dir, "glm")
GLM_INSTALL_DIR = r"C:\ENV\glm"

SDL3_MIXER_DIR = os.path.join(current_dir, "SDL_mixer")
SDL3_MIXER_INSTALL_DIR = r"C:\ENV\SDL3_mixer"






def extract_dir(zip_path, target_dir, without_top_dir=True):
    if not without_top_dir:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(target_dir)
        return

    with zipfile.ZipFile(zip_path, 'r') as zip_ref:
        # 获取所有文件
        names = zip_ref.namelist()

        # 推断 ZIP 顶层目录（例如 SDL3-devel-3.2.26-VC/）
        top_level_prefix = names[0].split('/')[0] + '/'

        for member in names:
            # 去掉顶层路径
            dest_path = member
            if dest_path.startswith(top_level_prefix):
                dest_path = dest_path[len(top_level_prefix):]

            # 忽略空路径
            if not dest_path:
                continue

            # 构建绝对路径
            final_path = os.path.join(target_dir, dest_path)

            # 创建目录
            if member.endswith('/'):
                os.makedirs(final_path, exist_ok=True)
            else:
                os.makedirs(os.path.dirname(final_path), exist_ok=True)
                with zip_ref.open(member) as src, open(final_path, 'wb') as dst:
                    dst.write(src.read())


# 解压并安装 SDL3
def install_sdl3():
    print(f"Installing SDL3 to {SDL3_INSTALL_DIR}")
    if not os.path.exists(SDL3_INSTALL_DIR):
        os.makedirs(SDL3_INSTALL_DIR)

    for file in tqdm.tqdm(os.listdir(SDL3_DIR)):
        if not file.endswith(".zip"):
            continue
        zip_file = os.path.join(SDL3_DIR, file)
        extract_dir(zip_file, SDL3_INSTALL_DIR, without_top_dir=True)


    print("SDL3 installed successfully.")

    return [SDL3_INSTALL_DIR, os.path.join(SDL3_INSTALL_DIR, 'lib', 'x64')]



# glm安装
def install_glm(release=True):
    if not os.path.exists(GLM_DIR):
        git_cmd = f"git clone https://github.com/g-truc/glm.git {GLM_DIR}"
        os.system(git_cmd)

    build_type = "Release" if release else "Debug"
    print("Installing glm...")
    stages = [
        f'cmake -S . -B build -DCMAKE_INSTALL_PREFIX="{GLM_INSTALL_DIR}"',
        f'cmake --build build --config {build_type}',
        f'cmake --install build --config {build_type}'
    ]
    
    # 切换到glm目录
    os.chdir(GLM_DIR)

    # 执行安装命令
    for stage in stages:
        os.system(stage)

    print("glm installed successfully.")


    return [GLM_INSTALL_DIR, os.path.join(GLM_INSTALL_DIR, 'lib')]


if __name__ == '__main__':
    print("Start to install thirdparty environment.")

    

    need_add_env_pathes = []

    ps = install_sdl3()
    need_add_env_pathes.extend(ps)

    ps = install_glm(release=True)
    need_add_env_pathes.extend(ps)

    ps = install_sdl3_mixer(release=True)
    need_add_env_pathes.extend(ps)

    print("All thirdparty environment installed successfully.")
    print("Please add the following environment variables to your system PATH:")
    for p in need_add_env_pathes:
        print(f"{p}")
