import sys
import os
import re
import glob


def df_to_csv(df: list, filename: str):
    if len(df) == 0:
        print("Dataframe is empty")
        return

    with open(filename, "w") as f:
        header = ",".join(df[0].keys())
        f.write(header + "\n")
        for row in sorted(df, key=lambda item: item["name"]):
            f.write(",".join(row.values()) + "\n")

if __name__ == '__main__':
    if len(sys.argv) >= 3:
        output_folder = sys.argv[1]
        results_file = sys.argv[2]
    else:
        output_folder = os.path.join(os.path.dirname(os.path.realpath(__file__)), "output")
        results_file = "results.csv"

    # df = {pd.DataFrame(columns=["name", "time"])}
    df = []
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
                        time_ms = re.search(r'\d*ms', line).group(0)
                        break

        df.append({"name": input_file, "time": str(time_ms)})

    df_to_csv(df, results_file)


