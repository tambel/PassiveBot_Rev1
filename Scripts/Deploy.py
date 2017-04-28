import os
import shutil

source_dir="D:\Bot\Projects\PassiveBot\Addons"

destination_dir="D:\Bot\World of Warcraft\Interface\AddOns"

def copy_all_dirs_content(src, dst):

    folders = [n for n in os.listdir(src) if os.path.isdir(os.path.join(src,n))]
    for f in folders:
        spath=os.path.join(src,f)
        dpath=os.path.join(dst,f)
        try:
            if os.path.isdir(dpath) and os.path.exists(dpath):
                shutil.rmtree(dpath)
        except FileExistsError as e:
            shutil.rmtree('/folder_name')
        finally:
            shutil.copytree(spath,dpath,False)


copy_all_dirs_content(source_dir,destination_dir)
