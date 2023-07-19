from scapy.all import *

# Função para capturar pacotes Wi-Fi de uma rede específica (SSID)
def capture_wifi_packets(ssid):
    def packet_handler(packet):
        if packet.haslayer(Dot11Beacon) or packet.haslayer(Dot11ProbeResp):
            if packet.info.decode() == ssid:
                print(f"SSID: {packet.info.decode()}, BSSID: {packet.addr2}")

    print(f"Capturando pacotes da rede Wi-Fi: {ssid}")
    sniff(iface="wlan0", prn=packet_handler)

# Função para verificar a senha da rede Wi-Fi
def verify_password(ssid, password):
    # Implemente aqui a lógica para verificar a senha
    # Pode ser através do mesmo algoritmo genético usado anteriormente

# Exemplo de uso
if __name__ == "__main__":
    ssid = input("Digite o nome da rede Wi-Fi (SSID): ")
    password = input("Digite a possível senha da rede Wi-Fi: ")

    # Inicia a captura de pacotes Wi-Fi
    capture_wifi_packets(ssid)

    # Verifica a senha da rede Wi-Fi
    verify_password(ssid, password)

