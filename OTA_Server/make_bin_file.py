ecu_id = 3
version = 2

with open("Sapaghetti.bin", "wb") as f:
    f.write(f"{ecu_id}\n".encode())      # 첫 줄: ECU ID (ex: "1\n")
    f.write(f"{version}\n".encode())     # 두 번째 줄: 버전 (ex: "2\n")