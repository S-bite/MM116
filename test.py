import subprocess
import json


def gen_args():
    with open("args.txt", "w") as f:
        for i in range(1, 3):
            f.write(f"{i} -1 -1\n")
# cmd = ["parallel", "--result", "result", "--colsep", " ", "-a", "args.txt", "./a.out", "{1}", "{2}",
#      "{3}"]


def exec_parallel():
    cmd = ["parallel", "--colsep", " ", "-a",
           "args.txt", "./a.out", "{1}", "{2}", "{3}"]
    # ret = subprocess.run(cmd, capture_output=True)
    ret = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    outputs_list = ret.stdout.decode("utf-8").split("\n")[:-1]
    print(outputs_list)
    outputs_dict = {}
    for output in outputs_list:
        seed, score, loop = output.split(" ")
        outputs_dict[f"seed{seed}"] = {
            "score": float(score), "loop": int(loop)}
    return outputs_dict


def write_json(data):
    try:
        with open("submissions.json", "r") as f:
            json_data = json.load(f)
    except Exception as e:
        print(e)
        print("Use empty json")
        json_data = {"nextid": 0, "submissions": {}}
    id = json_data["nextid"]
    json_data["nextid"] += 1
    json_data["submissions"][f"{id}"] = data
    with open("submissions.json", "w") as f:
        json.dump(json_data, f)


if __name__ == "__main__":
    gen_args()
    data = exec_parallel()
    write_json(data)
