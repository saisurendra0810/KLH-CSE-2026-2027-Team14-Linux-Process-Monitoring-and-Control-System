from flask import Flask, render_template, jsonify, request
import psutil
import time

app = Flask(__name__)


def health_score(info):
    """Return a simple 0-100 process health score based on live resource usage."""
    score = 100.0
    cpu = float(info.get("cpu", 0))
    memory = float(info.get("memory", 0))
    status = str(info.get("status", "")).lower()

    if cpu > 80:
        score -= min(35, (cpu - 80) * 0.7)
    elif cpu > 50:
        score -= (cpu - 50) * 0.2

    if memory > 20:
        score -= min(35, (memory - 20) * 0.8)
    elif memory > 10:
        score -= (memory - 10) * 0.15

    if status in {"zombie", "dead"}:
        score -= 45
    elif status in {"stopped", "suspended"}:
        score -= 20

    return max(0, min(100, round(score)))


def get_process_info(proc):
    try:
        with proc.oneshot():
            info = {
                "pid": proc.pid,
                "name": proc.name(),
                "username": proc.username(),
                "status": proc.status(),
                "cpu": round(proc.cpu_percent(interval=None), 1),
                "memory": round(proc.memory_percent(), 2),
                "ppid": proc.ppid(),
            }
            info["health"] = health_score(info)
            return info
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
        return None


def process_snapshot():
    result = []
    for proc in psutil.process_iter():
        info = get_process_info(proc)
        if info is not None:
            result.append(info)
    return result


@app.route("/")
def home():
    return render_template("index.html")


@app.route("/api/processes")
def processes():
    search = request.args.get("search", "").strip().lower()
    result = process_snapshot()

    if search:
        result = [
            p for p in result
            if search in p["name"].lower() or search == str(p["pid"])
        ]

    result.sort(key=lambda x: x["pid"])
    return jsonify(result)


@app.route("/api/process/<int:pid>")
def process_details(pid):
    try:
        proc = psutil.Process(pid)
        info = get_process_info(proc)
        if info is None:
            return jsonify({"error": "Process is no longer available"}), 404

        try:
            info["create_time"] = time.strftime(
                "%Y-%m-%d %H:%M:%S", time.localtime(proc.create_time())
            )
        except (psutil.AccessDenied, psutil.NoSuchProcess):
            info["create_time"] = "Unavailable"

        try:
            info["exe"] = proc.exe()
        except (psutil.AccessDenied, psutil.NoSuchProcess):
            info["exe"] = "Unavailable"

        try:
            info["cmdline"] = " ".join(proc.cmdline()) or "Unavailable"
        except (psutil.AccessDenied, psutil.NoSuchProcess):
            info["cmdline"] = "Unavailable"

        return jsonify(info)

    except psutil.NoSuchProcess:
        return jsonify({"error": "Process not found"}), 404
    except psutil.AccessDenied:
        return jsonify({"error": "Access denied"}), 403


@app.route("/api/process/<int:pid>/control", methods=["POST"])
def control_process(pid):
    data = request.get_json(silent=True) or {}
    action = data.get("action", "").lower()

    try:
        proc = psutil.Process(pid)
        if action == "terminate":
            proc.terminate()
            message = f"Termination signal sent to PID {pid}"
        elif action == "kill":
            proc.kill()
            message = f"Kill signal sent to PID {pid}"
        elif action == "suspend":
            proc.suspend()
            message = f"Process {pid} suspended"
        elif action == "resume":
            proc.resume()
            message = f"Process {pid} resumed"
        else:
            return jsonify({"error": "Invalid control action"}), 400
        return jsonify({"success": True, "message": message})
    except psutil.NoSuchProcess:
        return jsonify({"error": "Process not found"}), 404
    except psutil.AccessDenied:
        return jsonify({"error": "Access denied. You may need appropriate permissions."}), 403
    except Exception as exc:
        return jsonify({"error": str(exc)}), 500


@app.route("/api/system")
def system_info():
    memory = psutil.virtual_memory()
    swap = psutil.swap_memory()
    processes = process_snapshot()
    return jsonify({
        "cpu": round(psutil.cpu_percent(interval=0.1), 1),
        "memory": round(memory.percent, 1),
        "total_memory": round(memory.total / (1024 ** 3), 2),
        "used_memory": round(memory.used / (1024 ** 3), 2),
        "available_memory": round(memory.available / (1024 ** 3), 2),
        "swap": round(swap.percent, 1),
        "process_count": len(psutil.pids()),
        "running_count": sum(1 for p in processes if p["status"] == psutil.STATUS_RUNNING),
        "health_average": round(sum(p["health"] for p in processes) / len(processes), 1) if processes else 100,
        "uptime": round(time.time() - psutil.boot_time()),
    })


if __name__ == "__main__":
    # 0.0.0.0 makes the dashboard reachable from another device on the same network.
    app.run(host="0.0.0.0", port=5000, debug=True)
