import os
import subprocess

tp_cnt = 0  # True positive (correctly identified a real fall) count
fn_cnt = 0  # False negative (failed to identify a real fall) count

tn_cnt = 0  # True negative (correctly determine there are zero falls) count
fp_cnt = 0  # False positive (incorrectly identify falls when there aren't any) count


for sub in os.listdir("MobiFall_Dataset_v2.0"):
    if sub != "Readme.txt":
        if "FALLS" in os.listdir("MobiFall_Dataset_v2.0/%s" % sub):
            for trace_type in os.listdir("MobiFall_Dataset_v2.0/%s/FALLS" % sub):
                for trace in os.listdir("MobiFall_Dataset_v2.0/%s/FALLS/%s" % (sub, trace_type)):
                    if "acc" in trace:
                        retval = subprocess.call(["./FallDetectorTest", "MobiFall_Dataset_v2.0/%s/FALLS/%s/%s" % (sub, trace_type, trace)])
                        if retval > 0:
                            tp_cnt += 1
                        else:
                            fn_cnt += 1
        if "ADL" in os.listdir("MobiFall_Dataset_v2.0/%s" % sub):
            for trace_type in os.listdir("MobiFall_Dataset_v2.0/%s/ADL" % sub):
                for trace in os.listdir("MobiFall_Dataset_v2.0/%s/ADL/%s" % (sub, trace_type)):
                    if "acc" in trace:
                        retval = subprocess.call(["./FallDetectorTest", "MobiFall_Dataset_v2.0/%s/ADL/%s/%s" % (sub, trace_type, trace)])
                        if retval > 0:
                            fp_cnt += retval  # Increases by the number of FPs rather than 1
                        else:
                            tn_cnt += 1
print tp_cnt, fn_cnt, tn_cnt, fp_cnt
