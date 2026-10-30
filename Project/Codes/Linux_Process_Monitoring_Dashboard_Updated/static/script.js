let selectedPid = null;
let cpuHistory = [];
let memoryHistory = [];
const MAX_POINTS = 24;

const $ = id => document.getElementById(id);

function showSection(id, button) {
    document.querySelectorAll('.nav-item').forEach(item => item.classList.remove('active'));
    button.classList.add('active');
    $(id).scrollIntoView({ behavior: 'smooth', block: 'start' });
}

function healthClass(score) {
    if (score >= 80) return 'healthy';
    if (score >= 50) return 'watch';
    return 'critical';
}

function healthLabel(score) {
    if (score >= 80) return 'Healthy';
    if (score >= 50) return 'Watch';
    return 'Critical';
}

function drawChart(canvasId, values, lineLabel) {
    const canvas = $(canvasId);
    const rect = canvas.getBoundingClientRect();
    const dpr = window.devicePixelRatio || 1;
    const width = Math.max(300, rect.width);
    const height = 210;
    canvas.width = width * dpr;
    canvas.height = height * dpr;
    const ctx = canvas.getContext('2d');
    ctx.scale(dpr, dpr);
    ctx.clearRect(0, 0, width, height);

    const left = 42, right = 14, top = 14, bottom = 28;
    const plotW = width - left - right;
    const plotH = height - top - bottom;

    ctx.strokeStyle = '#e8edf5';
    ctx.lineWidth = 1;
    ctx.font = '11px Arial';
    ctx.fillStyle = '#8b96a8';
    for (let i = 0; i <= 4; i++) {
        const y = top + plotH - (plotH * i / 4);
        ctx.beginPath(); ctx.moveTo(left, y); ctx.lineTo(width - right, y); ctx.stroke();
        ctx.fillText(`${i * 25}%`, 7, y + 4);
    }

    if (!values.length) return;
    const points = values.map((v, i) => ({
        x: left + (values.length === 1 ? plotW : plotW * i / (values.length - 1)),
        y: top + plotH - (Math.max(0, Math.min(100, v)) / 100) * plotH
    }));

    const gradient = ctx.createLinearGradient(0, top, 0, top + plotH);
    gradient.addColorStop(0, 'rgba(37, 99, 235, .22)');
    gradient.addColorStop(1, 'rgba(37, 99, 235, 0)');
    ctx.beginPath();
    ctx.moveTo(points[0].x, top + plotH);
    points.forEach(p => ctx.lineTo(p.x, p.y));
    ctx.lineTo(points[points.length - 1].x, top + plotH);
    ctx.closePath(); ctx.fillStyle = gradient; ctx.fill();

    ctx.beginPath();
    points.forEach((p, i) => i ? ctx.lineTo(p.x, p.y) : ctx.moveTo(p.x, p.y));
    ctx.strokeStyle = '#2563eb'; ctx.lineWidth = 2.5; ctx.stroke();

    const last = points[points.length - 1];
    ctx.beginPath(); ctx.arc(last.x, last.y, 4, 0, Math.PI * 2); ctx.fillStyle = '#2563eb'; ctx.fill();
    ctx.fillStyle = '#64748b'; ctx.fillText(lineLabel, width - 78, height - 7);
}

async function loadSystem() {
    try {
        const response = await fetch('/api/system');
        const data = await response.json();

        $('cpu').textContent = `${data.cpu}%`;
        $('memory').textContent = `${data.memory}%`;
        $('processCount').textContent = data.process_count;
        $('sideProcessCount').textContent = data.process_count;
        $('tableProcessCount').textContent = data.process_count;
        $('runningCount').textContent = `${data.running_count} currently running`;
        $('healthAverage').textContent = `${data.health_average}`;
        $('memoryMeta').textContent = `${data.used_memory} GB used of ${data.total_memory} GB`;
        $('totalMemory').textContent = `${data.total_memory} GB`;
        $('usedMemory').textContent = `${data.used_memory} GB`;
        $('availableMemory').textContent = `${data.available_memory} GB`;
        $('memoryChartValue').textContent = `${data.memory}%`;
        $('cpuChartValue').textContent = `${data.cpu}%`;
        $('cpuCurrent').textContent = `${data.cpu}%`;

        cpuHistory.push(data.cpu); memoryHistory.push(data.memory);
        if (cpuHistory.length > MAX_POINTS) cpuHistory.shift();
        if (memoryHistory.length > MAX_POINTS) memoryHistory.shift();
        $('sampleCount').textContent = cpuHistory.length;
        drawChart('cpuChart', cpuHistory, 'CPU');
        drawChart('memoryChart', memoryHistory, 'Memory');
    } catch (error) {
        console.error('System update failed:', error);
    }
}

async function loadProcesses() {
    const search = $('search').value;
    try {
        const response = await fetch('/api/processes?search=' + encodeURIComponent(search));
        const processes = await response.json();
        const table = $('processTable');
        if (!processes.length) {
            table.innerHTML = '<tr><td colspan="8" class="loading">No matching processes found.</td></tr>';
            return;
        }

        table.innerHTML = processes.map(p => `
            <tr class="${selectedPid === p.pid ? 'selected-row' : ''}" onclick="selectProcess(${p.pid})">
                <td class="pid">${p.pid}</td>
                <td class="process-name"><strong>${escapeHtml(p.name)}</strong></td>
                <td>${escapeHtml(p.username || 'N/A')}</td>
                <td><span class="status-badge">${escapeHtml(p.status)}</span></td>
                <td>${p.cpu}%</td>
                <td>${p.memory}%</td>
                <td><span class="health-badge ${healthClass(p.health)}"><i></i>${p.health} · ${healthLabel(p.health)}</span></td>
                <td><button class="view-btn" onclick="event.stopPropagation(); selectProcess(${p.pid})">View</button></td>
            </tr>`).join('');

        renderHealthList(processes);
    } catch (error) {
        $('processTable').innerHTML = '<tr><td colspan="8" class="loading">Unable to load processes.</td></tr>';
    }
}

function renderHealthList(processes) {
    const top = [...processes].sort((a, b) => a.health - b.health).slice(0, 12);
    $('healthList').innerHTML = top.map(p => `
        <button class="health-row" onclick="selectProcess(${p.pid})">
            <div class="health-process"><span class="process-dot ${healthClass(p.health)}"></span><div><strong>${escapeHtml(p.name)}</strong><small>PID ${p.pid} · ${escapeHtml(p.status)}</small></div></div>
            <div class="health-meter"><div style="width:${p.health}%"></div></div>
            <span class="health-score ${healthClass(p.health)}">${p.health}</span>
        </button>`).join('');
}

async function selectProcess(pid) {
    selectedPid = pid;
    await showSelectedDetails();
    loadProcesses();
}

async function showSelectedDetails() {
    if (!selectedPid) return;
    try {
        const response = await fetch(`/api/process/${selectedPid}`);
        const data = await response.json();
        if (data.error) { showMessage(data.error); return; }

        $('selectedTitle').textContent = `${data.name} · PID ${data.pid}`;
        $('selectedSubtitle').textContent = 'Live process details from the machine running this dashboard.';
        $('selectedDetails').className = 'selected-details';
        $('selectedDetails').innerHTML = `
            <div class="selected-health"><div class="score-ring ${healthClass(data.health)}"><span>${data.health}</span><small>Health</small></div><div><h3>${healthLabel(data.health)}</h3><p>Based on CPU, memory and process state.</p></div></div>
            <div class="detail-grid">
                ${detail('PID', data.pid)}${detail('Parent PID', data.ppid)}${detail('User', data.username || 'N/A')}${detail('Status', data.status)}
                ${detail('CPU Usage', `${data.cpu}%`)}${detail('Memory Usage', `${data.memory}%`)}${detail('Created', data.create_time)}${detail('Executable', data.exe)}
                ${detail('Command Line', data.cmdline, true)}
            </div>`;
    } catch (error) { showMessage('Unable to retrieve process details.'); }
}

function detail(label, value, wide = false) {
    return `<div class="detail ${wide ? 'wide' : ''}"><span>${label}</span><strong>${escapeHtml(value)}</strong></div>`;
}

async function controlSelected(action) {
    if (!selectedPid) { showMessage('Please select a process first.'); return; }
    if ((action === 'terminate' || action === 'kill') && !confirm(`Are you sure you want to ${action} PID ${selectedPid}?`)) return;
    try {
        const response = await fetch(`/api/process/${selectedPid}/control`, { method: 'POST', headers: {'Content-Type': 'application/json'}, body: JSON.stringify({action}) });
        const data = await response.json(); showMessage(data.message || data.error);
        setTimeout(() => { loadProcesses(); loadSystem(); showSelectedDetails(); }, 600);
    } catch (error) { showMessage('Operation failed.'); }
}

function showMessage(text) {
    $('message').innerHTML = `<div class="msg">${escapeHtml(text)}</div>`;
    setTimeout(() => $('message').innerHTML = '', 3500);
}

function escapeHtml(value) {
    return String(value).replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('>','&gt;').replaceAll('"','&quot;').replaceAll("'",'&#039;');
}

$('search').addEventListener('input', loadProcesses);
window.addEventListener('resize', () => { drawChart('cpuChart', cpuHistory, 'CPU'); drawChart('memoryChart', memoryHistory, 'Memory'); });

loadSystem();
loadProcesses();
setInterval(() => { loadSystem(); loadProcesses(); if (selectedPid) showSelectedDetails(); }, 3000);
