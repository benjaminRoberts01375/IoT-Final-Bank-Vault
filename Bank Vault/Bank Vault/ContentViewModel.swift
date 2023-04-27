// Ben Roberts

import SwiftUI

final class ContentViewModel: ObservableObject {
    /// Tracker for showing login page
    @Published var showLoginPage: Bool
    /// Tracker for being in edit mode
    @Published var editMode: Bool
    
    init() {
        self.editMode = UserM.shared.vaults.isEmpty
        self.showLoginPage = true
        VaultManagerM.shared.connectMQTT()
    }
    
    /// Checks the user's ID to see if the login page should be dismissed
    internal func determineLoginStatus(newId: String?) {
        print("User id: \(UserM.shared.userID ?? "")")
        if newId != nil {
            showLoginPage = false
        }
    }
}
