// Ben Roberts

import Combine
import SwiftUI

final class VaultManagerM: ObservableObject {
    // Setup singleton
    public static var shared = VaultManagerM()
    private init() {
        self.vaultsToConfigure = []
        self.userVaults = []
        self.mqtt = MQTTManagerM()
    }
    
    @Published public var mqtt: MQTTManagerM
    
    /// Vaults in need of being configured
    @Published public var vaultsToConfigure: [VaultM] {
        didSet {
            self.objectWillChange.send()
        }
    }
    /// Vaults already setup
    @Published public var userVaults: [VaultM] {
        didSet {
            self.objectWillChange.send()
        }
    }
    
    /// Resets the vaultsToConfigure list and begins stream of vaults that need configuring
    public func checkVaultsToConfigure() {
        self.vaultsToConfigure = []
        mqtt.checkVaultsToSetup()
    }
    
    public func connectMQTT() {
        mqtt.connect()
    }
    
    public func addVault(vault: VaultM) {
        mqtt.setupVault(vault)
    }
}
