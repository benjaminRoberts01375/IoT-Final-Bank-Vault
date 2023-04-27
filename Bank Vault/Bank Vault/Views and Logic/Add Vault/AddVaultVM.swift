// Ben Roberts

import Combine
import SwiftUI

final class AddVaultVM: ObservableObject {
    private var cancellables: Set<AnyCancellable>
    
    init() {
        cancellables = []
        
        VaultManagerM.shared.objectWillChange.sink { _ in
            DispatchQueue.main.async {
                 self.objectWillChange.send()
             }
        }
        .store(in: &cancellables)
        
        checkForVaults()
    }
    
    internal func checkForVaults() {
        VaultManagerM.shared.checkVaultsToConfigure()
    }
}
