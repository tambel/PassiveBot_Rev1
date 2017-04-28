import os


ida_path="C:\Program Files (x86)\IDA 6.8"

idaq_exe_path=os.path.join(ida_path,"idaq.exe")

ida_script_path="D:\Bot\Projects\PassiveBot\Scripts\GetNamesFromIDB.py"

ida_database_path="D:\Bot\IDB_archive\\7.2.0\wow.idb"

command_string="{} -A -S\"{}\" \"{}\"".format(idaq_exe_path, ida_script_path, ida_database_path)

os.system(command_string)

#idaq.exe -A -S"D:\Bot\Projects\PassiveBot\Scripts\GetNamesFromIDB.py" "D:\Bot\World of Warcraft\Wow.idb"