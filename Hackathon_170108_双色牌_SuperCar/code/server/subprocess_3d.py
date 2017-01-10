import subprocess

bundler_command_line = 'C:/WinPython-64bit-2.7.10.3/python-2.7.10.amd64/python.exe F:/osm-bundler-pmvs2-cmvs-full-32-64/osm-bundler/osm-bundlerWin64/RunBundler.py --photos="E:/code/python/Huawei_LiteOS/received"'
pmvs_command_line = 'C:/WinPython-64bit-2.7.10.3/python-2.7.10.amd64/python.exe F:/osm-bundler-pmvs2-cmvs-full-32-64/osm-bundler/osm-bundlerWin64/RunPMVS.py --bundlerOutputPath="F:/workspace/osm-bundler"'

print('Start')
bundler_return_code = subprocess.call(bundler_command_line)
pmvs_return_code = subprocess.call(pmvs_command_line)
print('Finish')