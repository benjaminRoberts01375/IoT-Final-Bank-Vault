// Ben Roberts

import SwiftUI

final class ContentViewModel: ObservableObject {
    @Published var editMode: Bool
    
    init() {
        self.editMode = UserLoginM.shared.vaults.isEmpty
    }
}
