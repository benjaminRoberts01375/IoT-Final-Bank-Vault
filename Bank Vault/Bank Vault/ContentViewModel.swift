// Ben Roberts

import Combine
import SwiftUI

final class ContentViewModel: ObservableObject {
    /// Tracker for showing login page
    @Published var showLoginPage: Bool
    // Handling state change
    private var cancellables: Set<AnyCancellable>
    
    init() {
        self.showLoginPage = true
        VaultManagerM.shared.connectMQTT()
        
        cancellables = []
               
        VaultManagerM.shared.objectWillChange.sink { _ in
            DispatchQueue.main.async {
                self.objectWillChange.send()
            }
        }
        .store(in: &cancellables)
    }
    
    /// Checks the user's ID to see if the login page should be dismissed
    internal func determineLoginStatus(newId: String?) {
        print("User id: \(UserM.shared.userID ?? "")")
        if newId != nil {
            showLoginPage = false
        }
    }
}
