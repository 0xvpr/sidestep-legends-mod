#!/usr/bin/env python3


# Created by:       0xvpr
# Created:          May 5th, 2024

# Updated by:       0xvpr
# Updated:          May 5th, 2024

# Description:      Download third party sources (ImGUI - Copyright (c) 2014-2024 Omar Cornut)


import os
import shutil
import requests


source = [
    'imconfig.h',
    'imgui.cpp',
    'imgui.h',
    'imgui_demo.cpp',
    'imgui_draw.cpp',
    'imgui_internal.h',
    'imgui_tables.cpp',
    'imgui_widgets.cpp',
    'imstb_rectpack.h',
    'imstb_textedit.h',
    'imstb_truetype.h'
]

backends = [
    'imgui_impl_gdi.cpp',
    'imgui_impl_gdi.h'
]
backends2 = [
    'imgui_impl_win32.cpp',
    'imgui_impl_win32.h'
]
to_download = [
    f'https://github.com/ocornut/imgui/raw/master/{_x}' for _x in source
] + [
    f'https://raw.githubusercontent.com/MouriNaruto/imgui/master/examples/{_x}' for _x in backends
] + [
    f'https://raw.githubusercontent.com/ocornut/imgui/master/backends/{_x}' for _x in backends2
]
imgui_path = os.path.join(os.getcwd(), 'include/imgui')


def download(url: str) -> None:
    resp = requests.get(url)
    f = open(os.path.join(imgui_path, url.split('/')[-1]), 'wb')
    f.write(resp.content)
    f.close()

def fix() -> None:
    f = open(os.path.join(imgui_path, 'imgui.h'), 'r').read().split('\n')
    for num, line in enumerate(f):
        strip = line.strip().replace(' ', '')
        if strip == '#defineIM_COL32_R_SHIFT0':
            f[num] = '#define IM_COL32_R_SHIFT    16'
            continue
        if strip == '#defineIM_COL32_B_SHIFT16':
            f[num] = '#define IM_COL32_B_SHIFT    0'
            continue
    out = open(os.path.join(imgui_path, 'imgui.h'), 'w')
    out.write('\n'.join(f))
    out.close()


if __name__ == '__main__':
    if os.path.isdir(imgui_path):
        shutil.rmtree(imgui_path)
    os.mkdir(imgui_path)
    for num, url in enumerate(to_download):
        print(f"{str(round(num / len(to_download) * 100)) + '%':5s} - downloading '{to_download}'...")
        download(url)
    fix()
    print('100%')
