// Ben Roberts

import Combine
import MQTTNIO
import SwiftUI

final class MQTTManagerM {
    let broker: String
    let port: Int
    var topic: String
    var setupTopic: String
    var mqtt: MQTTClient?
    var cancellablePublisher: AnyCancellable?
    var cancellableConnection: AnyCancellable?
    
    init() {
        self.broker = "broker.emqx.io"
        self.port = 1883
        self.setupTopic = "setup"
        self.topic = "setup"
    }
    
    private func connect() -> MQTTClient {
        let mqtt = MQTTClient(                          // MQTT Configuration
            configuration: .init(
                target: .host(broker, port: port),          // Host and port
                protocolVersion: .version5,                 // MQTT v5
                clientId: "clientId-iOS",                   // Client ID
                clean: true,                                // I dunno
                credentials: .none,                         // No credentials
                willMessage: .none,                         // No leaving message?
                keepAliveInterval: .seconds(60),            // Timeout
                reschedulePings: true                       // Do ping when other messages are sent
            ),
            eventLoopGroupProvider: .createNew
        )
        
        mqtt.connect()                                                                                          // Connect to configured MQTT broker
        
        
        return mqtt
    }
    
}
