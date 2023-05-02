// Ben Roberts

import Combine
import SwiftUI

final class VaultInfoVM: ObservableObject {
    /// Vault to display info for
    let vault: VaultM
    // Handling state change
    private var cancellables: Set<AnyCancellable>
    
    init(vault: VaultM) {
        self.vault = vault
        
        cancellables = []
               
        VaultManagerM.shared.objectWillChange.sink { _ in
            DispatchQueue.main.async {
                self.objectWillChange.send()
            }
        }
        .store(in: &cancellables)
    }
    
    internal func removeVault() {
        VaultManagerM.shared.removeVault(vault)
    }
    
    internal func openDoor() {
        VaultManagerM.shared.openVault(vault)
    }
    
    internal func closeDoor() {
        VaultManagerM.shared.closeVault(vault)
    }
    
    internal func addDevice() {
        VaultManagerM.shared.addDeviceToVault(vault)
    }
}
