// Ben Roberts

import SwiftUI

final class UserM: ObservableObject {
    // Setup singleton
    public static var shared: UserM = UserM()
    private init() { }
    
    @Published var userID: String?
}
