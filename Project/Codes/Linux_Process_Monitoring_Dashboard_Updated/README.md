# Linux Process Monitoring Dashboard

A Flask + psutil web dashboard for live CPU, memory and process monitoring.

## Run

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 app.py
```

Open `http://127.0.0.1:5000/` on the same laptop.

The server binds to `0.0.0.0`, so the dashboard can also be reached from another device on the same local network using the laptop's IP address and port `5000`.

## Dashboard features

- Left-side navigation for Memory Usage, CPU Usage, Processes and Process Health.
- Live CPU and memory graphs.
- Current CPU, memory and process counts.
- Running process list collected from the machine using psutil.
- Search by process name or PID.
- Click any process to display PID, parent PID, user, status, CPU, memory, executable and command line.
- Per-process health score from 0-100 based on CPU load, memory usage and process state.
- Average process health score.
- Suspend, resume, terminate and kill controls.
- Automatic refresh every 3 seconds.

Run it on the laptop you want to monitor. Linux is recommended for the Linux-focused process controls; some operations require suitable permissions.
