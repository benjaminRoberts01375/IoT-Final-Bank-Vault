// Ben Roberts

import Combine
import MQTTNIO
import SwiftUI

final class MQTTManagerM: ObservableObject {
    let broker: String
    let port: Int
    var topic: String
    var setupTopic: String
    var mqtt: MQTTClient?
    var cancellablePublisher: AnyCancellable?
    var cancellableConnected: AnyCancellable?
    var cancellableDisconnected: AnyCancellable?
    var cancellableReconnecting: AnyCancellable?
    @Published var connectionStatus: ConnectionStatus
    
    init() {
        self.broker = "broker.emqx.io"
        self.port = 1883
        self.setupTopic = "setup"
        self.topic = "setup"
        self.connectionStatus = .disconnected
    }
    
    public enum ConnectionStatus {
        case connected
        case reconnecting
        case disconnected
    }
    
    public func connect() {
        print("Connecting to MQTT")
        let mqtt = MQTTClient(                          // MQTT Configuration
            configuration: .init(
                target: .host(broker, port: port),          // Host and port
                protocolVersion: .version5,                 // MQTT v5
                clientId: "theIOSDevice",                   // Client ID
                clean: true,                                // I dunno
                credentials: .none,                         // No credentials
                willMessage: .none,                         // No leaving message?
                keepAliveInterval: .seconds(60),            // Timeout
                reschedulePings: true                       // Do ping when other messages are sent
            ),
            eventLoopGroupProvider: .createNew
        )
        
        mqtt.connect()                                                                                          // Connect to configured MQTT broker
        
        self.cancellablePublisher = mqtt.messagePublisher                                                       // Setup background updates for messages
            .sink { message in
                DispatchQueue.main.async {                                                                          // Run on main thread
                    guard let rawData:Data = message.payload.string?.data(using: .utf8) else { return }                 // Convert message to data type
                    print(String(decoding: rawData, as: UTF8.self))                                                     // Debug message
                    if let decodedData = try? JSONDecoder().decode(ArdunioNeedsSetup.self, from: rawData) {             // Decode JSON into ArduinoSetupRequest struct
                        if decodedData.setupResponse == "Needs setup" {
                            if !VaultManagerM.shared.vaultsToConfigure.contains(where: { $0.id == decodedData.vault.id }) {
                                VaultManagerM.shared.vaultsToConfigure.append(decodedData.vault)                            // Add the vault to the list of vaults in need of configuring
                            }
                        }
                    }
                    else if let decodedData = try? JSONDecoder().decode(ArduinoIsSetup.self, from: rawData) {            // Decode JSON into ArduinoSetupRequest struct
                        if decodedData.confirmSetup &&
                            VaultManagerM.shared.vaultsToConfigure.contains(where: { $0.id == decodedData.vault.id }) &&
                            decodedData.phoneID == UserM.shared.userID
                        {
                            guard let userID = UserM.shared.userID else { return }
                            decodedData.vault.accessor = userID
                            VaultManagerM.shared.userVaults.append(decodedData.vault)                            // Add the vault to the list of vaults in need of configuring
                            VaultManagerM.shared.checkVaultsToConfigure()
                        }
                    }
                }
            }
        
        self.cancellableConnected = mqtt.connectPublisher                                                       // Setup background updates for successful connection
            .sink(receiveValue: { connection in
                DispatchQueue.main.async {                                                                          // Run on main thread
                    print("Conencted to broker")
                    self.connectionStatus = .connected
                    self.objectWillChange.send()
                }
            })
        self.cancellableDisconnected = mqtt.disconnectPublisher                                                 // Setup background updates for successful connection
            .sink(receiveValue: { connection in
                DispatchQueue.main.async {                                                                          // Run on main thread
                    print("Disconnected from broker")
                    self.connectionStatus = .disconnected
                    self.objectWillChange.send()
                }
            })
        self.cancellableReconnecting = mqtt.reconnectPublisher                                                  // Setup background updates for successful connection
            .sink(receiveValue: { connection in
                DispatchQueue.main.async {                                                                          // Run on main thread
                    print("Reconnecting to broker")
                    self.connectionStatus = .reconnecting
                    self.objectWillChange.send()
                }
            })
        
        self.mqtt = mqtt
    }
    
    public func connectToSetup() {
        self.mqtt?.subscribe(to: setupTopic, qos: .exactlyOnce)
    }
    
    private func sendData(data: Encodable, topic: String) {
        print("Sending data")
        guard let json = try? JSONEncoder().encode(data) else { return }
        guard let mqtt = self.mqtt else { return }
        mqtt.publish(String(decoding: json, as: UTF8.self), to: topic, qos: .atLeastOnce)
    }
    
    public func checkVaultsToSetup() {
        connectToSetup()
        sendData(data: CheckUnconfiguredVaults(), topic: setupTopic)
    }
    
    public func setupVault(_ vault: VaultM) {
//        connectToSetup()
        sendData(data: RequestToSetupVault(vault: vault), topic: setupTopic)
    }
}
