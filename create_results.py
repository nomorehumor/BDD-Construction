import sys
import pandas as pd
import os
import re
import glob

if __name__ == '__main__':
    if len(sys.argv) > 1:
        output_folder = sys.argv[1]
    else:
        output_folder = "C:\dev\Bachelor\code\cmake-build-debug\output"
    
    df = pd.DataFrame(columns=["name", "time"])
    run_folders = os.listdir(output_folder)
    for run_folder in run_folders:
        input_file = ""
        done = True
        time_ms = -1
        log_file = glob.glob(os.path.join(output_folder, run_folder, "*.log"))[0]
        with open(log_file, "r") as f:
            lines = f.readlines()
            input_file = re.search(r'(/.*)*.txt', lines[0]).group(1)

            if re.search(r'[warning]]', lines[len(lines)-1]):
                done = False
            else:
                for line in reversed(lines):
                    time_line = re.search(r'(Ruleset BDD generated in | BDD generated in)', line)
                    if time_line:
                        time_ms = re.search(r'\d*ms', line).group(1)
                        break

        df.append({"name": input_file, "time": time_ms})

    df.to_csv("results.csv")


