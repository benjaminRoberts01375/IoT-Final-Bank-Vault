// Ben Roberts

import SwiftUI

final class UserLoginM: ObservableObject {
    // Setup singleton
    public static var shared: UserLoginM = UserLoginM()
    private init() {
        self.vaults = []
    }
    
    @Published var vaults: [VaultM]
    @Published var userID: String?
}
