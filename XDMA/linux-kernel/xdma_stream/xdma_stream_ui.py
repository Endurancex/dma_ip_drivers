#!/usr/bin/env python3
import tkinter as tk
from tkinter import filedialog
import subprocess
import threading
import struct
import os
import queue
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

tool_path = "../tools"
stop_flag = False
canvas = None
plot_queue = queue.Queue()  # 用于线程安全传输绘图数据

# ====== GUI 更新函数（线程安全） ======
def append_log(msg):
    root.after(0, lambda: _append_log(msg))

def _append_log(msg):
    text_output.insert(tk.END, msg + "\n")
    text_output.see(tk.END)

def check_plot_queue():
    """定时检查后台线程传来的绘图数据"""
    try:
        while True:
            nums = plot_queue.get_nowait()
            fig, ax = plt.subplots(figsize=(12,5))  # 固定高度较小
            ax.plot(nums, marker='.', linestyle='-', markersize=2)
            ax.set_title("64-bit Data Waveform (256-bit blocks split)")
            ax.set_xlabel("Index")
            ax.set_ylabel("Value")
            ax.grid(True)

            global canvas
            if canvas:
                canvas.get_tk_widget().destroy()
            canvas = FigureCanvasTkAgg(fig, master=frame_plot)
            canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
            canvas.draw()
    except queue.Empty:
        pass
    root.after(100, check_plot_queue)

# ====== 系统命令执行 ======
def run_command(cmd):
    global stop_flag
    if stop_flag:
        return "[INFO] Execution stopped by user."
    try:
        result = subprocess.check_output(cmd, shell=True, stderr=subprocess.STDOUT)
        return result.decode()
    except subprocess.CalledProcessError as e:
        return e.output.decode()

# ====== 线程包装 ======
def run_in_thread(func):
    global stop_flag
    stop_flag = False
    threading.Thread(target=func).start()

# ====== FPGA 操作 ======
def capture_and_axis():
    global stop_flag
    try:
        ch = int(entry_ch.get())
        transferSz = entry_size.get()
        transferCount = int(entry_count.get())
        file_path = entry_file.get()

        append_log("=== Start Capture (C2H) ===")
        for i in range(ch):
            if stop_flag:
                append_log("[INFO] Capture stopped by user.")
                return
            cmd = f"{tool_path}/dma_from_device -d /dev/xdma0_c2h_{i} -s {transferSz} -f {file_path} -c {transferCount}"
            append_log(run_command(cmd))

        append_log("=== Perform AXIS Data Generation ===")
        transferSz_int = int(transferSz, 16 if str(transferSz).startswith("0x") else 10)
        packet_len = transferSz_int // 32
        cmds = [
            f"{tool_path}/reg_rw /dev/xdma0_user 0x0 w {packet_len}",
            f"{tool_path}/reg_rw /dev/xdma0_user 0x0 w",
            f"{tool_path}/reg_rw /dev/xdma0_user 0x8 w 1",
            f"{tool_path}/reg_rw /dev/xdma0_user 0x8 w",
            f"{tool_path}/reg_rw /dev/xdma0_user 0x8 w 0",
            f"{tool_path}/reg_rw /dev/xdma0_user 0x8 w"
        ]
        for cmd in cmds:
            if stop_flag:
                append_log("[INFO] AXIS Data Gen stopped by user.")
                return
            append_log(run_command(cmd))

        append_log("=== Capture + AXIS Done ===")
    except Exception as e:
        append_log(f"[ERROR] Capture + AXIS failed: {e}")

def data_send():
    global stop_flag
    try:
        ch = int(entry_ch.get())
        transferSz = entry_size.get()
        transferCount = int(entry_count.get())
        file_path = entry_file.get()

        append_log("=== Start Sending (H2C) ===")
        for i in range(ch):
            if stop_flag:
                append_log("[INFO] Send stopped by user.")
                return
            cmd = f"{tool_path}/dma_to_device -d /dev/xdma0_h2c_{i} -s {transferSz} -f {file_path} -c {transferCount}"
            append_log(run_command(cmd))
        append_log("=== Sending Done ===")
    except Exception as e:
        append_log(f"[ERROR] Sending failed: {e}")

def analyze_and_plot_thread():
    global stop_flag
    try:
        file_path = entry_file.get()
        if not os.path.exists(file_path):
            append_log(f"[WARN] File {file_path} does not exist, skipping analysis.")
            return

        append_log(f"=== Analyzing captured data: {file_path} ===")
        with open(file_path, "rb") as f:
            data = f.read()
            block_count = len(data) // 32  # 每 256-bit 为一个 block
            nums = []
            for i in range(block_count):
                if stop_flag:
                    append_log("[INFO] Analysis stopped by user.")
                    return
                values = struct.unpack_from("QQQQ", data, i*32)  # 拆成 4 个 64-bit
                nums.extend(values)


        plot_queue.put(nums)

    except Exception as e:
        append_log(f"[ERROR] Failed to analyze data: {e}")

# ====== STOP ======
def stop_execution():
    global stop_flag
    stop_flag = True
    append_log("[INFO] Stop signal sent.")

# ====== 文件选择 ======
def choose_file():
    file_path = filedialog.asksaveasfilename(initialdir="data", defaultextension=".bin")
    if file_path:
        entry_file.delete(0, tk.END)
        entry_file.insert(0, file_path)

# ====== GUI 布局 ======
root = tk.Tk()
root.title("FPGA PCIe Control GUI")
root.geometry("1000x800")

# 参数输入区
frame_params = tk.Frame(root)
frame_params.grid(row=0, column=0, columnspan=5, padx=10, pady=5, sticky="w")

tk.Label(frame_params, text="Channel Count:").grid(row=0, column=0, sticky="w")
entry_ch = tk.Entry(frame_params, width=6)
entry_ch.insert(0, "1")
entry_ch.grid(row=0, column=1, sticky="w", padx=5)

tk.Label(frame_params, text="Transfer Size:").grid(row=0, column=2, sticky="w")
entry_size = tk.Entry(frame_params, width=10)
entry_size.insert(0, "0x8000")
entry_size.grid(row=0, column=3, sticky="w", padx=5)

tk.Label(frame_params, text="Transfer Count:").grid(row=0, column=4, sticky="w")
entry_count = tk.Entry(frame_params, width=6)
entry_count.insert(0, "1")
entry_count.grid(row=0, column=5, sticky="w", padx=5)

tk.Label(frame_params, text="File Path:").grid(row=1, column=0, sticky="w", pady=5)
entry_file = tk.Entry(frame_params, width=50)
entry_file.insert(0, "data/c2h_data.bin")
entry_file.grid(row=1, column=1, columnspan=3, sticky="w")
tk.Button(frame_params, text="Choose File", command=choose_file).grid(row=1, column=4, sticky="w", padx=5)

# 操作按钮
frame_buttons = tk.Frame(root)
frame_buttons.grid(row=2, column=0, columnspan=5, pady=5, sticky="w")

tk.Button(frame_buttons, text="Capture + AXIS Data Gen", width=25, command=lambda: run_in_thread(capture_and_axis)).grid(row=0, column=0, padx=5)
tk.Button(frame_buttons, text="Send Data (H2C)", width=15, command=lambda: run_in_thread(data_send)).grid(row=0, column=1, padx=5)
tk.Button(frame_buttons, text="Analyze & Plot Data", width=20, command=lambda: run_in_thread(analyze_and_plot_thread)).grid(row=0, column=2, padx=5)
tk.Button(frame_buttons, text="STOP", width=10, fg="red", command=stop_execution).grid(row=0, column=3, padx=5)

# Text 输出区域在上方
frame_text = tk.Frame(root, bd=2, relief=tk.SUNKEN)
frame_text.grid(row=3, column=0, columnspan=5, padx=10, pady=5, sticky="nsew")
text_output = tk.Text(frame_text, height=20)
text_output.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
scrollbar = tk.Scrollbar(frame_text, command=text_output.yview)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
text_output.config(yscrollcommand=scrollbar.set)

# 绘图区域在下方，固定高度
frame_plot = tk.Frame(root, bd=2, relief=tk.SUNKEN, height=300)
frame_plot.grid(row=4, column=0, columnspan=5, padx=10, pady=5, sticky="ew")
frame_plot.grid_propagate(False)

# 行列权重
root.grid_rowconfigure(3, weight=1)  # Text 可拉伸
root.grid_rowconfigure(4, weight=0)  # 绘图固定
root.grid_columnconfigure(0, weight=1)

if not os.path.exists("data"):
    os.makedirs("data")

# 启动定时检查绘图队列
root.after(100, check_plot_queue)

root.mainloop()
