// Ben Roberts

import AuthenticationServices
import SwiftUI

struct UserLoginV: View {
    @Environment(\.colorScheme) var colorScheme
    let controller: UserLoginVM = UserLoginVM()
    
    var body: some View {
        SignInWithAppleButton(
            .signIn,
            onRequest: { $0.requestedScopes = [] },
            onCompletion: { controller.login(result: $0) }
        )
        .signInWithAppleButtonStyle(colorScheme == .dark ? .white : .black)
        .frame(width: 250, height: 60)
        .cornerRadius(200)
    }
}

struct UserLoginV_Previews: PreviewProvider {
    static var previews: some View {
        UserLoginV()
    }
}
