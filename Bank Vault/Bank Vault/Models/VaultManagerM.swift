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
    
    public func addVaultInteraction(_ interaction: ArduinoInteracted) {
        if let vault = userVaults.first(where: { $0.id == interaction.vault.id }) {                     // Check if we have the vault setup on our end
            if !(vault.history?.contains(where: { $0.time == interaction.doorStatus.time }) ?? true) {      // If the timecode doesn't exist in our records
                print("Added \(interaction.doorStatus.time)")
                vault.history?.append(interaction.doorStatus)                                                   // Add it
            }
        }
    }
}
