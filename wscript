
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

import os.path
import json

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')
    
    def generate_appinfo(src, target):
        print("Genrating appinfo.")
        json_data=open(src)
        data = json.load(json_data)

        f = open(target,'w')
        f.write('#ifndef appinfo_h\n')
        f.write('#define appinfo_h\n')
        f.write('#define VERSION_LABEL "' + data["versionLabel"] + '"\n') 
        f.write('#define COMPANY_NAME "' + data["companyName"] + '"\n') 
        f.write('#define APPLICATION_NAME "' + data["longName"] + '"\n') 
        f.write('#define APPLICATION_UUID "' + data["uuid"] + '"\n') 
        f.write('#endif\n')
        f.close()

    if not os.path.isdir("build/generated"):
        os.makedirs("build/generated")
    generate_appinfo("appinfo.json", "build/generated/appinfo.h")


def build(ctx):
    ctx.load('pebble_sdk')

    build_worker = os.path.exists('worker_src')
    binaries = []
    
    for p in ctx.env.TARGET_PLATFORMS:
        ctx.set_env(ctx.all_envs[p])
        ctx.set_group(ctx.env.PLATFORM_NAME)
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)
        ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'),
        target=app_elf)

        if build_worker:
            worker_elf='{}/pebble-worker.elf'.format(ctx.env.BUILD_DIR)
            binaries.append({'platform': p, 'app_elf': app_elf, 'worker_elf': worker_elf})
            ctx.pbl_worker(source=ctx.path.ant_glob('worker_src/**/*.c'),
            target=worker_elf)
        else:
            binaries.append({'platform': p, 'app_elf': app_elf})

    ctx.set_group('bundle')
    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))
