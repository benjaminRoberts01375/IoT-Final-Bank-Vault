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
    
    /// Boiler plate function for connecting to MQTT
    public func connectMQTT() {
        mqtt.connect()
    }
    
    /// Boilerplate function for completing the setup for a vault
    /// - Parameter vault: Vault to setup
    public func addVault(vault: VaultM) {
        mqtt.setupVault(vault)
    }
    
    /// Adds to the history of a vault
    /// - Parameter interaction: Interaction taken from a vault
    public func addVaultInteraction(_ interaction: ArduinoInteracted) {
        if let vault = userVaults.first(where: { $0.id == interaction.vault.id }) {                     // Check if we have the vault setup on our end
            if !(vault.history?.contains(where: { $0.time == interaction.doorStatus.time }) ?? true) {      // If the timecode doesn't exist in our records
                vault.history?.append(interaction.doorStatus)                                                   // Add it
                self.objectWillChange.send()
            }
        }
    }
    
    /// Removes a connected vault
    /// - Parameter vault: Vault to remove
    public func removeVault(_ vault: VaultM) {
        if userVaults.contains(where: { $0.id == vault.id } ) {
            mqtt.removeVault(vault)
            userVaults.removeAll(where: { $0.id == vault.id } )
        }
    }
    
    /// Open a vault
    /// - Parameter vault: Vault to open
    public func openVault(_ vault: VaultM) {
            mqtt.openVault(vault)
    }
    
    /// Closes a vault
    /// - Parameter vault: vault to close
    public func closeVault(_ vault: VaultM) {
            mqtt.closeVault(vault)
    }
    
    /// Adds a device to a vault
    /// - Parameter vault: Vault to enter setup for
    public func addDeviceToVault(_ vault: VaultM) {
            mqtt.addDeviceToVault(vault)
    }
}
