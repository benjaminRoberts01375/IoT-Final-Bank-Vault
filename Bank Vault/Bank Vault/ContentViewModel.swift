// Ben Roberts

import SwiftUI

final class ContentViewModel: ObservableObject {
    /// Tracker for showing login page
    @Published var showLoginPage: Bool
    /// Tracker for being in edit mode
    @Published var editMode: Bool
    
    init() {
        self.editMode = UserLoginM.shared.vaults.isEmpty
        self.showLoginPage = true
    }
}
