const form = document.getElementById('simulationForm');
const referenceInput = document.getElementById('referenceString');
const frameInput = document.getElementById('frameCount');
const errorMessage = document.getElementById('errorMessage');
const summaryCards = document.getElementById('summaryCards');
const simulationTable = document.getElementById('simulationTable');
const algorithmDescription = document.getElementById('algorithmDescription');
const comparisonNote = document.getElementById('comparisonNote');
const chartCanvas = document.getElementById('faultChart');

const descriptions = {
  FIFO: 'FIFO (First In, First Out) replaces the page that has been in memory the longest.',
  LRU: 'LRU (Least Recently Used) replaces the page that has not been accessed for the longest time in the past.',
  Optimal: 'Optimal replaces the page whose next use is farthest in the future, or a page that will never be used again.'
};

let results = {};
let currentAlgorithm = 'FIFO';

function parseInput() {
  const raw = referenceInput.value.trim();
  if (!raw) throw new Error('Please enter a page reference string.');
  const tokens = raw.split(/[,\s]+/).filter(Boolean);
  if (tokens.length > 100) throw new Error('Please use no more than 100 page requests for this visualizer.');
  if (!tokens.every(token => /^\d+$/.test(token))) {
    throw new Error('Use non-negative whole-number page values separated by commas or spaces.');
  }
  const pages = tokens.map(Number);
  const frames = Number(frameInput.value);
  if (!Number.isInteger(frames) || frames < 1 || frames > 10) {
    throw new Error('The number of frames must be an integer from 1 to 10.');
  }
  return { pages, frames };
}

function simulateFIFO(pages, frameCount) {
  const frames = [];
  const steps = [];
  let pointer = 0;
  let faults = 0;
  let hits = 0;

  pages.forEach(page => {
    const hit = frames.includes(page);
    if (hit) {
      hits++;
    } else {
      faults++;
      if (frames.length < frameCount) {
        frames.push(page);
      } else {
        frames[pointer] = page;
        pointer = (pointer + 1) % frameCount;
      }
    }
    steps.push({ page, frames: snapshot(frames, frameCount), hit, fault: !hit });
  });
  return { steps, faults, hits };
}

function simulateLRU(pages, frameCount) {
  const frames = [];
  const lastUsed = new Map();
  const steps = [];
  let faults = 0;
  let hits = 0;

  pages.forEach((page, index) => {
    const hit = frames.includes(page);
    if (hit) {
      hits++;
    } else {
      faults++;
      if (frames.length < frameCount) {
        frames.push(page);
      } else {
        let leastRecentPage = frames[0];
        for (const candidate of frames) {
          if (lastUsed.get(candidate) < lastUsed.get(leastRecentPage)) {
            leastRecentPage = candidate;
          }
        }
        frames[frames.indexOf(leastRecentPage)] = page;
      }
    }
    lastUsed.set(page, index);
    steps.push({ page, frames: snapshot(frames, frameCount), hit, fault: !hit });
  });
  return { steps, faults, hits };
}

function simulateOptimal(pages, frameCount) {
  const frames = [];
  const steps = [];
  let faults = 0;
  let hits = 0;

  pages.forEach((page, index) => {
    const hit = frames.includes(page);
    if (hit) {
      hits++;
    } else {
      faults++;
      if (frames.length < frameCount) {
        frames.push(page);
      } else {
        let replaceIndex = 0;
        let farthestNextUse = -1;
        frames.forEach((candidate, frameIndex) => {
          const nextUse = pages.indexOf(candidate, index + 1);
          if (nextUse === -1) {
            replaceIndex = frameIndex;
            farthestNextUse = Infinity;
          } else if (farthestNextUse !== Infinity && nextUse > farthestNextUse) {
            replaceIndex = frameIndex;
            farthestNextUse = nextUse;
          }
        });
        frames[replaceIndex] = page;
      }
    }
    steps.push({ page, frames: snapshot(frames, frameCount), hit, fault: !hit });
  });
  return { steps, faults, hits };
}

function snapshot(frames, frameCount) {
  return Array.from({ length: frameCount }, (_, index) =>
    index < frames.length ? frames[index] : null
  );
}

function runSimulation() {
  errorMessage.textContent = '';
  try {
    const { pages, frames } = parseInput();
    results = {
      FIFO: simulateFIFO(pages, frames),
      LRU: simulateLRU(pages, frames),
      Optimal: simulateOptimal(pages, frames)
    };
    renderSummary(pages.length);
    renderChart();
    renderTable(currentAlgorithm);
  } catch (error) {
    errorMessage.textContent = error.message;
  }
}

function renderSummary(totalRequests) {
  const configs = [
    { key: 'FIFO', mark: 'F', className: 'mark-fifo' },
    { key: 'LRU', mark: 'L', className: 'mark-lru' },
    { key: 'Optimal', mark: 'O', className: 'mark-optimal' }
  ];
  const bestFaults = Math.min(...configs.map(item => results[item.key].faults));
  summaryCards.innerHTML = configs.map(item => {
    const result = results[item.key];
    const hitRate = totalRequests ? ((result.hits / totalRequests) * 100).toFixed(1) : '0.0';
    const best = result.faults === bestFaults;
    return `<article class="summary-card">
      <div class="summary-top">
        <span class="summary-name">${item.key}</span>
        <span class="algo-mark ${item.className}">${item.mark}</span>
      </div>
      <div class="fault-number">${result.faults}</div>
      <div class="fault-label">Total page faults ${best ? '· Lowest count' : ''}</div>
      <div class="card-stats">
        <span>Hits: <strong>${result.hits}</strong></span>
        <span>Hit rate: <strong>${hitRate}%</strong></span>
      </div>
    </article>`;
  }).join('');
  const winners = configs.filter(item => results[item.key].faults === bestFaults).map(item => item.key);
  comparisonNote.textContent = winners.length === 1
    ? `${winners[0]} produced the fewest page faults for this input (${bestFaults}).`
    : `Tie: ${winners.join(' and ')} produced the fewest page faults for this input (${bestFaults}).`;
}

function renderChart() {
  const ctx = chartCanvas.getContext('2d');
  const width = chartCanvas.clientWidth || 600;
  const height = chartCanvas.clientHeight || 260;
  const dpr = window.devicePixelRatio || 1;
  chartCanvas.width = width * dpr;
  chartCanvas.height = height * dpr;
  ctx.scale(dpr, dpr);
  ctx.clearRect(0, 0, width, height);

  const labels = ['FIFO', 'LRU', 'Optimal'];
  const values = labels.map(label => results[label].faults);
  const colors = ['#41b987', '#4d83e8', '#9b75dc'];
  const padding = { top: 18, right: 20, bottom: 40, left: 38 };
  const plotWidth = width - padding.left - padding.right;
  const plotHeight = height - padding.top - padding.bottom;
  const maxValue = Math.max(1, ...values);
  const tickCount = Math.min(5, maxValue);
  ctx.font = '12px DM Sans, sans-serif';
  ctx.textAlign = 'right';
  ctx.textBaseline = 'middle';

  for (let i = 0; i <= tickCount; i++) {
    const value = Math.round(maxValue * i / tickCount);
    const y = padding.top + plotHeight - (value / maxValue) * plotHeight;
    ctx.strokeStyle = '#e9edf5';
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(padding.left, y);
    ctx.lineTo(width - padding.right, y);
    ctx.stroke();
    ctx.fillStyle = '#7b879b';
    ctx.fillText(String(value), padding.left - 9, y);
  }

  const band = plotWidth / labels.length;
  const barWidth = Math.min(74, band * .48);
  labels.forEach((label, index) => {
    const barHeight = values[index] / maxValue * plotHeight;
    const x = padding.left + band * index + (band - barWidth) / 2;
    const y = padding.top + plotHeight - barHeight;
    ctx.fillStyle = colors[index];
    roundedRect(ctx, x, y, barWidth, Math.max(barHeight, 2), 7);
    ctx.fill();
    ctx.fillStyle = '#23314d';
    ctx.font = '700 13px DM Sans, sans-serif';
    ctx.textAlign = 'center';
    ctx.fillText(String(values[index]), x + barWidth / 2, Math.max(10, y - 10));
    ctx.fillStyle = '#596780';
    ctx.font = '600 12px DM Sans, sans-serif';
    ctx.fillText(label, x + barWidth / 2, height - 17);
  });
}

function roundedRect(ctx, x, y, width, height, radius) {
  const r = Math.min(radius, width / 2, height / 2);
  ctx.beginPath();
  ctx.moveTo(x + r, y);
  ctx.arcTo(x + width, y, x + width, y + height, r);
  ctx.arcTo(x + width, y + height, x, y + height, r);
  ctx.arcTo(x, y + height, x, y, r);
  ctx.arcTo(x, y, x + width, y, r);
  ctx.closePath();
}

function renderTable(algorithm) {
  currentAlgorithm = algorithm;
  const result = results[algorithm];
  algorithmDescription.textContent = descriptions[algorithm];
  document.querySelectorAll('.tab').forEach(tab => {
    const active = tab.dataset.algorithm === algorithm;
    tab.classList.toggle('active', active);
    tab.setAttribute('aria-selected', String(active));
  });

  const pages = result.steps.map(step => step.page);
  let table = '<table class="sim-table"><thead><tr><th class="row-label">Page request</th>';
  pages.forEach((page, index) => {
    table += `<th>${page}<br><small style="margin:2px 0 0">#${index + 1}</small></th>`;
  });
  table += '</tr></thead><tbody>';

  const frameCount = Number(frameInput.value);
  for (let frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    table += `<tr><th class="row-label">Frame ${frameIndex + 1}</th>`;
    result.steps.forEach(step => {
      const value = step.frames[frameIndex];
      const empty = value === null;
      table += `<td class="frame-cell ${empty ? 'empty-cell' : ''}">${empty ? '—' : value}</td>`;
    });
    table += '</tr>';
  }

  table += '<tr><th class="row-label">Status</th>';
  result.steps.forEach(step => {
    table += `<td class="${step.hit ? 'page-hit' : 'page-fault'}">${step.hit ? 'HIT' : 'FAULT'}</td>`;
  });
  table += '</tr><tr><th class="row-label">Fault count</th>';
  let runningFaults = 0;
  result.steps.forEach(step => {
    if (step.fault) runningFaults++;
    table += `<td>${runningFaults}</td>`;
  });
  table += '</tr></tbody></table>';
  simulationTable.innerHTML = table;
}

form.addEventListener('submit', event => {
  event.preventDefault();
  runSimulation();
});

document.querySelectorAll('.tab').forEach(tab => {
  tab.addEventListener('click', () => {
    if (results[tab.dataset.algorithm]) renderTable(tab.dataset.algorithm);
  });
});

document.querySelectorAll('.example-button').forEach(button => {
  button.addEventListener('click', () => {
    referenceInput.value = button.dataset.sequence;
    frameInput.value = button.dataset.frames;
    runSimulation();
  });
});

let resizeTimer;
window.addEventListener('resize', () => {
  clearTimeout(resizeTimer);
  resizeTimer = setTimeout(() => {
    if (results.FIFO) renderChart();
  }, 100);
});

runSimulation();
