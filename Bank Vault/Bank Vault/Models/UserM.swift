// Ben Roberts

import SwiftUI

final class UserM: ObservableObject {
    // Setup singleton
    public static var shared: UserM = UserM()
    private init() {
        self.vaults = []
    }
    
    @Published var vaults: [VaultM]
    @Published var userID: String?
}
